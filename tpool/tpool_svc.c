#include "tpoolsrv.h"

static int to_get_ctx(T_Connect *conn,T_NetHead *nethead)
{
T_SRV_Var *srvp=(T_SRV_Var *)conn->Var;;
GDA *gp=(GDA *)srvp->var;
char msg[200];
sdbcfunc app;
//ȡctx_id,��nethead->O_NODE��
CTX_stu *ctxp;
int ret;

        unset_callback(srvp->TCB_no);
        app=conn->only_do;
        conn->only_do=NULL;
        if(!srvp->SQL_Connect) {
                sprintf(msg,"ȡ���ݿ����ӳ�ʧ�ܣ�,pool[%d]",srvp->poolno);
		nethead->ERRNO1=-200;
		nethead->ERRNO2=PACK_NOANSER;
		return_error(conn,nethead,msg);
                ShowLog(1,"%s:%s",__FUNCTION__,msg);
                return 0;
        }
        ctxp=get_ctx(nethead->O_NODE);// ��ʱ�������ݿ�
        if(!ctxp&&nethead->ERRNO2!=PACK_NOANSER) {
                release_DB_connect(&srvp->SQL_Connect,srvp->poolno);
		conn->timeout=srvp->o_timeout;
		sprintf(msg,"ȡCTXʧ�ܣ�PROTO=%d,ctx_id=%u",nethead->PROTO_NUM,nethead->O_NODE);
		nethead->ERRNO1=-198;
		nethead->ERRNO2=PACK_NOANSER;
                return_error(conn,nethead,msg);
		ShowLog(1,"%s:%s",__FUNCTION__,msg);
		return 0;
        }
	if(conn->status) {
		conn->timeout=75;
	}
	gp->ctx=*ctxp;
	sprintf(gp->ShowID,"%s:%d",gp->ctx.DEVID,srvp->TCB_no);

        ret=app(conn,nethead);
        if(!conn->status || nethead->ERRNO2==PACK_NOANSER) {
//��״̬��״̬ز��
                release_DB_connect(&srvp->SQL_Connect,srvp->poolno);
		sprintf(gp->ShowID,"%s:%d",gp->devid,srvp->TCB_no);
		conn->status=0;
		conn->timeout=srvp->o_timeout;
		gp->ctx.ctx_id=0;
		*gp->ctx.DEVID=0;
        }
        return ret;
}

//service��  
//����Ӧ�ô������ݿ����ӳ�
//״̬���������
int tpool_svc(T_Connect *conn,T_NetHead *nethead,sdbcfunc app)
{
T_SRV_Var *sp;
char msg[200];
int ret;

        sp=(T_SRV_Var *)conn->Var;
	conn->status=0;
//	ShowLog(2,"%s:%s",__FUNCTION__,nethead->data);
	conn->status = nethead->ERRNO2==PACK_STATUS; 
	if(!sp->SQL_Connect) {
		conn->only_do=app;
        	ret=DB_connect_MGR(sp->TCB_no,sp->poolno,&sp->SQL_Connect,to_get_ctx);
       		if(ret==0) return to_get_ctx(conn,nethead);
       		if(ret==1) return THREAD_ESCAPE;//DB_connect_MGR�����˻ص�����

		sprintf(msg,"ȡ���ݿ����ӳ�ʧ��!,pool[%d],ret=%d",sp->poolno,ret);
        	nethead->ERRNO1=-200;
        	nethead->ERRNO2=PACK_NOANSER;
		conn->status=0;
		conn->only_do=0;
		return_error(conn,nethead,msg);
		ShowLog(1,"%s:%s",__FUNCTION__,msg);
		return 0;
	}
//������ݿ��Ѵ��ڣ�˵����״̬�У�����ȡctx�ˣ�ֱ��ִ��app
	ret=app(conn,nethead);
	if(!conn->status||nethead->ERRNO2==PACK_NOANSER) {
	GDA *gp=(GDA *)sp->var;
//ִ��ʧ�ܣ�Ҫ��ز������
		release_DB_connect(&sp->SQL_Connect,sp->poolno);
		conn->status=0;
		conn->timeout=sp->o_timeout;
		sprintf(gp->ShowID,"%s:%d",gp->devid,sp->TCB_no);
		gp->ctx.ctx_id=0;
		*gp->ctx.DEVID=0;
	}
	return ret;
}

int pageselect_svc(T_Connect *conn,T_NetHead *nethead)
{
	return tpool_svc(conn,nethead,page_select);
}

int dui_svc(T_Connect *conn,T_NetHead *nethead)
{
	return tpool_svc(conn,nethead,dui);
}

int select_svc(T_Connect *conn,T_NetHead *nethead)
{
	return tpool_svc(conn,nethead,SQL_Select);
}

//������״̬����
int prepare_svc(T_Connect *conn,T_NetHead *nethead)
{
	if(nethead->ERRNO2 != PACK_STATUS) {
	char msg[200];
	T_SRV_Var *sp=(T_SRV_Var *)conn->Var;
		release_DB_connect(&sp->SQL_Connect,sp->poolno);
		conn->timeout=sp->o_timeout;
                sprintf(msg,"ȱ��״̬��־");
                nethead->ERRNO1=-199;
		nethead->ERRNO2=conn->status?PACK_NOANSER:-1;//����״̬
        	nethead->PROTO_NUM=PutEvent(conn,65535 & nethead->PROTO_NUM);
		nethead->O_NODE=LocalAddr(conn->Socket,0);
		nethead->PKG_REC_NUM=0;
		nethead->data=msg;
		nethead->PKG_LEN=strlen(nethead->data);
		nethead->ERRNO1=SendPack(conn,nethead);
		ShowLog(1,"%s:%s",__FUNCTION__,msg);
		conn->status=0;
		return 0;
	}
	return tpool_svc(conn,nethead,SQL_Prepare);
}

//��״̬����
int close_svc(T_Connect *conn,T_NetHead *nethead)
{
T_SRV_Var *sp=(T_SRV_Var *)conn->Var;
	if(!sp->SQL_Connect) {
	char msg[200];
		nethead->ERRNO1=-200;
		nethead->ERRNO2=PACK_NOANSER;
		sprintf(msg,"���ݿ����Ӷ�ʧ");
		conn->status=0;
		return_error(conn,nethead,msg);
		ShowLog(1,"%s:%s",__FUNCTION__,msg);
		return 0;
	}
	return tpool_svc(conn,nethead,SQL_Close);
}

int transaction_svc(T_Connect *conn,T_NetHead *nethead)
{
T_SRV_Var *sp=(T_SRV_Var *)conn->Var;
char msg[200];

	if(nethead->ERRNO1==TRANBEGIN) {
		if(nethead->ERRNO2 != PACK_STATUS) {
			release_DB_connect(&sp->SQL_Connect,sp->poolno);
			conn->timeout=sp->o_timeout;
       		        sprintf(msg,"ȱ��״̬��־");
       	         	nethead->ERRNO1=-199;
			nethead->ERRNO2=PACK_NOANSER;//����״̬
			conn->status=0;
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
	return tpool_svc(conn,nethead,SQL_EndTran);
}

int exec_svc(T_Connect *conn,T_NetHead *nethead)
{
//Ҫ��ȡ DB ���ӳ�
	return tpool_svc(conn,nethead,SQL_Exec);
}

int rpc_svc(T_Connect *conn,T_NetHead *nethead)
{
	return tpool_svc(conn,nethead,SQL_RPC);
}

int ctx_login_svc(T_Connect *conn,T_NetHead *head)
{
T_SRV_Var *srvp=(T_SRV_Var *)conn->Var;
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
GDA *gp=(GDA *)srvp->var;
    if(gp->ctx.ctx_id&&gp->ctx.ctx_id!=head->PKG_REC_NUM) {
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
    ret=DB_connect_MGR(srvp->TCB_no,srvp->poolno,&srvp->SQL_Connect,ctx_login_app);
    if(ret==0) return ctx_login_app(conn,head);
    if(ret==1) return THREAD_ESCAPE;//DB_connect_MGR�����˻ص�����

    sprintf(tmp,"���ݿ�����ʧ��");
    head->ERRNO1=-200;
    head->ERRNO2=-1;
    return_error(conn,head,tmp);
    ShowLog(1,"%s:%s",__FUNCTION__,tmp);
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
		conn->status=0;
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
	sprintf(gp->ShowID,"%s:%d",gp->devid,srvp->TCB_no);
	return 0;
}

