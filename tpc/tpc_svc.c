#include "tpcsrv.h"

static int tpc_svc(T_Connect *conn,T_NetHead *nethead,sdbcfunc app)
{
T_SRV_Var *srvp=(T_SRV_Var *)conn->Var;
GDA *gp=(GDA *)srvp->var;
int ret;
	conn->status = nethead->ERRNO2==PACK_STATUS;
	if(!srvp->SQL_Connect) {
		ret=get_DB_connect(&srvp->SQL_Connect,srvp->poolno);
		if(ret) {
		char msg[200];
			sprintf(msg,"ȡ���ӳ�ʧ�ܣ�,DBpool[%d],ret=%d",srvp->poolno,ret);
			nethead->ERRNO1=-200;
			nethead->ERRNO2=PACK_NOANSER;
			conn->status=0;
			return_error(conn,nethead,msg);
        		ShowLog(1,"%s:%s",__FUNCTION__,msg);
			return 0;
		}
		CTX_stu *ctxp=get_ctx(nethead->O_NODE);// ��ʱ�������ݿ�
		if(!ctxp&&nethead->ERRNO2!=PACK_NOANSER) {
		char msg[200];
			release_DB_connect(&srvp->SQL_Connect,srvp->poolno);
       	        	sprintf(msg,"ȡCTXʧ�ܣ�PROTO=%d,ctx_id=%u",nethead->PROTO_NUM,nethead->O_NODE);
       	        	nethead->ERRNO1=-198;
       	        	nethead->ERRNO2=PACK_NOANSER;
       	        	return_error(conn,nethead,msg);
       	        	ShowLog(1,"%s:%s",__FUNCTION__,msg);
                	return 0;
		}
		if(conn->status) conn->timeout=75;//״̬�������75��ĳ�ʱ
		gp->ctx=*ctxp;
        	sprintf(gp->ShowID,"%s:%lX",gp->ctx.DEVID,srvp->tid);
	}
	ret=app(conn,nethead);
//ShowLog(5,"%s:status=%d,timeout=%d",__FUNCTION__,conn->status,conn->timeout);
	if(!conn->status || nethead->ERRNO2==PACK_NOANSER) {
		release_DB_connect(&srvp->SQL_Connect,srvp->poolno);
		conn->status=0;
		gp->ctx.ctx_id=0;
		*gp->ctx.DEVID=0;
		conn->timeout=srvp->o_timeout;//�ָ�ԭ�еĳ�ʱֵ
        	sprintf(gp->ShowID,"%s:%lX",gp->devid,srvp->tid);
	}
	return ret;
}

int select_svc(T_Connect *conn,T_NetHead *nethead)
{
	return tpc_svc(conn,nethead,SQL_Select);
}

int pageselect_svc(T_Connect *conn,T_NetHead *nethead)
{
	return tpc_svc(conn,nethead,page_select);
}

int prepare_svc(T_Connect *conn,T_NetHead *nethead)
{
T_SRV_Var *sp=(T_SRV_Var *)conn->Var;
int ret=0;
char msg[200];
	if(nethead->ERRNO2!=PACK_STATUS) {
//���ӳ�Ҫ�ͷ���
		release_DB_connect(&sp->SQL_Connect,sp->poolno);
		sprintf(msg,"ȱ��״̬��־");
		nethead->ERRNO1=-199;
		nethead->ERRNO2=-1;
		conn->timeout=sp->o_timeout;
		ret=return_error(conn,nethead,msg);
		ShowLog(1,"%s:%s",__FUNCTION__,msg);
		return ret;
	}
	return tpc_svc(conn,nethead,SQL_Prepare);
}

int exec_svc(T_Connect *conn,T_NetHead *nethead)
{
	return tpc_svc(conn,nethead,SQL_Exec);
}

int rpc_svc(T_Connect *conn,T_NetHead *nethead)
{
	return tpc_svc(conn,nethead,SQL_RPC);
}

int close_svc(T_Connect *conn,T_NetHead *nethead)
{
T_SRV_Var *srvp=(T_SRV_Var *)conn->Var;
int ret=0;
	if(!srvp->SQL_Connect) {
	char msg[200];
		strcpy(msg,"���ݿ����ӳض�ʧ��");
		conn->status=0;
       		nethead->ERRNO1=-1;
       		nethead->ERRNO2=PACK_NOANSER;
		ret=return_error(conn,nethead,msg);
		ShowLog(1,"%s:%s",__FUNCTION__,msg);
		return ret;
	}
	ret=tpc_svc(conn,nethead,SQL_Close);
	return ret;
}

int transaction_svc(T_Connect *conn,T_NetHead *nethead)
{
T_SRV_Var *sp=(T_SRV_Var *)conn->Var;
int ret=0;
char msg[200];
	if(nethead->ERRNO1 == TRANBEGIN) {
		if(nethead->ERRNO2 != PACK_STATUS) {
			release_DB_connect(&sp->SQL_Connect,sp->poolno);
       		        sprintf(msg,"ȱ��״̬��־");
       	         	nethead->ERRNO1=-199;
			nethead->ERRNO2=PACK_NOANSER;//����״̬
			conn->status=0;
			conn->timeout=sp->o_timeout;
			return_error(conn,nethead,msg);
			ShowLog(1,"%s:%s",__FUNCTION__,msg);
			return 0;
		}
	} else if(!sp->SQL_Connect) {
		nethead->ERRNO1=-200;
		sprintf(msg,"���ݿ����Ӷ�ʧ");
		nethead->ERRNO2=PACK_NOANSER;//����״̬
		return_error(conn,nethead,msg);
		ShowLog(1,"%s:%s",__FUNCTION__,msg);
		return 0;
	}
	ret=tpc_svc(conn,nethead,SQL_EndTran);
	return ret;
}

int dui_svc(T_Connect *conn,T_NetHead *nethead)
{
	return tpc_svc(conn,nethead,dui);
}

int ctx_login_svc(T_Connect *conn,T_NetHead *head)
{
T_SRV_Var *srvp=(T_SRV_Var *)conn->Var;
GDA *gp=(GDA*)srvp->var;
int ret;
char tmp[204];
    if(head->PKG_REC_NUM==-1) {
        strcpy(tmp,"��Ч��ctx_id -1");
        head->ERRNO1=-198;
        head->ERRNO2=-1;
        return_error(conn,head,tmp);
        ShowLog(1,"%s:%s",__FUNCTION__,tmp);
        return 0;
    }
/*
    if(gp->ctx.ctx_id && gp->ctx.ctx_id != head->PKG_REC_NUM) {
	sprintf(tmp,"�Ѿ���¼���������ٵ�¼");
	if(head->ERRNO2 != PACK_NOANSER) {
            head->ERRNO1=-198;
            head->ERRNO2=PACK_NOANSER;;
	    return_error(conn,head,tmp);
	}
        ShowLog(1,"%s:%s",__FUNCTION__,tmp);
        return 0;
    }
*/
//��֤CA�����¼Ŀ��DEVID������Կ

    //ȡ���ݿ����ӳ�
	ret=get_DB_connect(&srvp->SQL_Connect,srvp->poolno);
        if(ret) {

		sprintf(tmp,"���ݿ�����ʧ��");
		head->ERRNO1=-200;
		head->ERRNO2=-1;
		return_error(conn,head,tmp);
		ShowLog(1,"%s:%s",__FUNCTION__,tmp);
		return 0;
	}
	ret=ctx_login_app(conn,head);
	release_DB_connect(&srvp->SQL_Connect,srvp->poolno);
	conn->timeout=srvp->o_timeout;
	sprintf(gp->ShowID,"%s:%lX",gp->devid,srvp->tid);
	return ret;
}

int ctx_logout_svc(T_Connect *conn,T_NetHead *head)
{
T_SRV_Var *srvp=(T_SRV_Var *)conn->Var;
int ret;
GDA *gp=(GDA *)srvp->var;
char msg[200];
CTX_stu *ctxp;

        ctxp=get_ctx(head->O_NODE);// ��ʱ�������ݿ�
        if(srvp->SQL_Connect) {
                srvp->SQL_Connect->Errno=DBFAULT;
                release_DB_connect(&srvp->SQL_Connect,srvp->poolno);
		conn->timeout=srvp->o_timeout;
		if(conn->status) {
			conn->timeout=srvp->o_timeout;
			conn->status=0;
		}
        }
        if(!ctxp) {
                sprintf(msg,"ȡCTX %u ʧ�ܣ�",head->O_NODE);
                if(head->ERRNO2 != PACK_NOANSER) {
                        head->ERRNO1=-198;
                        head->ERRNO2=PACK_NOANSER;
                        return_error(conn,head,msg);
                }
                ShowLog(1,"%s:%s",__FUNCTION__,msg);
                return 0;
        }
        gp->ctx=*ctxp;
        ret=ctx_del(&gp->ctx);
        sprintf(msg,"%s logout flg=%d",gp->ctx.DEVID,ret);
        if(head->ERRNO2 != PACK_NOANSER) {
                head->ERRNO1=0;
                head->ERRNO2=ret;
                return_error(conn,head,msg);
        }
	gp->ctx.ctx_id=0;
	*gp->ctx.DEVID=0;
	ShowLog(2,"%s:tid=%lX,%s",__FUNCTION__,pthread_self(),msg);
        sprintf(gp->ShowID,"%s:%lX",gp->devid,srvp->tid);
        return 0;
}

