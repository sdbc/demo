#include <unistd.h>
#include "tpcsrv.h"
#include <dw.h>
#include <scry.h>
#include <enigma.h>
#include <crc.h>
#include <crc32.h>

extern srvfunc Function[];

#include <logs.tpl>
#include <logs.stu>

int login(T_Connect *conn,T_NetHead *NetHead)
{
int ret,crc;
char tmp[200];
char *cp,*key;
char tmp1[1024],cliaddr[20];
DWS dw;
login_stu logrec;//��¼�Ľṹ
log_stu logret;//���صĽṹ
ENIGMA2 egm;
FILE *fd;
T_SRV_Var *up;
GDA *gp;
//u_int e[RSALEN],m[RSALEN];

	up=(T_SRV_Var *)conn->Var;
	gp=(GDA *)up->var;
	StrAddr(NetHead->O_NODE,cliaddr);
//ShowLog(5,"%s:SDBC7.0 Client IP Addr=%s,Net_login %s",__FUNCTION__,cliaddr,NetHead->data);
	net_dispack(&logrec,NetHead->data,login_tpl);
	strcpy(gp->devid,logrec.DEVID);
	sprintf(gp->ShowID,"%s:%s:%lX",logrec.DEVID,cliaddr,up->tid);
	mthr_showid_add(up->tid,gp->ShowID);

	if(NetHead->ERRNO1>0) conn->MTU=NetHead->ERRNO1;
	if(NetHead->ERRNO2>0) conn->timeout=NetHead->ERRNO2;

	cp=getenv("KEYFILE");
	if(!cp||!*cp) {
		strcpy(tmp1,"ȱ�ٻ������� KEYFILE");
		NetHead->ERRNO1=-193;
errret:
		NetHead->ERRNO2=-1;
		return_error(conn,NetHead,tmp1);
		ShowLog(1,"%s:Error %s",__FUNCTION__,tmp1);
		return 0; // fail
	}
/* read key */
	crc=0;
reopen:
	ret=initdw(cp,&dw);
	if(ret) {
		if((errno==24)&& (++crc<5)) {
			sleep(15);
			goto reopen;
		}
		sprintf(tmp1,"Init dw[%s] error %d",cp,ret);
		NetHead->ERRNO1=-194;
		goto errret;
	}
	crc=ssh_crc32((unsigned char *)logrec.DEVID,strlen(logrec.DEVID));
	key=getdw(crc,&dw);
	if(!key) {
		freedw(&dw);
                sprintf(tmp1,"��Ч�� DEVID");
		NetHead->ERRNO1=-196;
                goto errret;
        }

//ShowLog(5,"getdw key=%s",key);
	enigma2_init(&egm,key,0);
/* check CA */
	memset(gp->operid,0,sizeof(gp->operid));
	cp=getenv("CADIR");
	if(!cp||!*cp) cp=".";
    if(strcmp(gp->devid,"REGISTER")) {
	strncpy(gp->operid,logrec.UID,sizeof(gp->operid)-1);
	sprintf(tmp,"%s/%s.CA",cp,logrec.DEVID);
ShowLog(5,"CAfile=%s,key=%s",tmp,key);
	fd=fopen(tmp,"r");
	if(!fd) {
		if(errno==2) {
		    crc=strlen(logrec.CA);
		    enigma2_encrypt(&egm,logrec.CA,crc);
		    byte_a64(tmp1,logrec.CA,crc);
//ShowLog(5,"CA=%s",tmp1);
		    fd=fopen(tmp,"w");
		    if(!fd) {
			sprintf(tmp1,"write %s err=%d",tmp,errno);
err1:
			freedw(&dw);
		NetHead->ERRNO1=-195;
			goto errret;
		    }
		    fprintf(fd,"%s\n",tmp1);
		    fclose(fd);
		} else {
			sprintf(tmp1,"open CAfile %s err=%d",tmp,errno);
		NetHead->ERRNO1=-195;
			goto err1;
		}
	} else {
		fgets(tmp1,sizeof(logrec.CA),fd);
		fclose(fd);
		TRIM(tmp1);
		ret=a64_byte(tmp,tmp1);
		enigma2_decrypt(&egm,tmp,ret);
		tmp[ret]=0;
		if(strcmp(tmp,logrec.CA)) {
			sprintf(tmp1,"CA ����");
//ShowLog(5,"%s CA=%s log=%s len=%d",tmp1,tmp,logrec.CA,ret);
			goto err1;
		}
	}
    } else {   //δע��ͻ���ע��
	char *keyD;
/* REGISTER label|CA|devfile|CHK_Code| */

ShowLog(2,"REGISTER %s",logrec.UID);
	if(!*logrec.UID) {
		sprintf(tmp1,"REGSTER is empty!");
		goto err1;
	}
//uid=devfile
	crc=0xFFFF&gencrc((unsigned char *)logrec.UID,strlen(logrec.UID));
//pwd=CHK_Code
	sscanf(logrec.PWD,"%04X",&ret);
	ret &= 0xFFFF;
	if(ret != crc) {
		sprintf(tmp1,"REGISTER:devfile CHK Code error! ");//, crc,ret);
		goto err1;
	}
	stptok(logrec.UID,logrec.DEVID,sizeof(logrec.DEVID),".");//logrec.DEVID=׼��ע���DEVID
	crc=ssh_crc32((unsigned char *)logrec.DEVID,strlen(logrec.DEVID));
	keyD=getdw(crc,&dw);
	if(!keyD) {
		sprintf(tmp1,"ע��ʧ��,%s:û������豸��",
				logrec.DEVID);
		goto err1;
	}
	enigma2_init(&egm,keyD,0);
	sprintf(tmp,"%s/%s.CA",cp,logrec.DEVID);
ShowLog(5,"REGISTER:%s",tmp);
	if(0!=(fd=fopen(tmp,"r"))) {
		fgets(tmp1,81,fd);
		fclose(fd);
		TRIM(tmp1);
		ret=a64_byte(tmp,tmp1);
		enigma2_decrypt(&egm,tmp,ret);
		tmp[ret]=0;
		if(strcmp(tmp,logrec.CA)) {
			sprintf(tmp1,"ע��ʧ��,%s �ѱ�ע��,ʹ���С�",
					logrec.DEVID);
			goto err1;
		}
	} else if(errno != 2) {
		sprintf(tmp1,"CA ����");
		goto err1;
	}
/*���豸������д���ļ�*/
	fd=fopen(tmp,"w");
	if(fd) {
	int len=strlen(logrec.CA);
		enigma2_encrypt(&egm,logrec.CA,len);
		byte_a64(tmp1,logrec.CA,len);
		fprintf(fd,"%s\n",tmp1);
		fclose(fd);
	}
	else ShowLog(1,"net_login:REGISTER open %s for write,err=%d,%s",
		tmp,errno,strerror(errno));

	freedw(&dw);
	sprintf(tmp,"%s/%s",cp,logrec.UID);
	fd=fopen(tmp,"r");
	if(!fd) {
		sprintf(tmp1,"REGISTER �򲻿��ļ� %s err=%d,%s",
					logrec.CA,errno,strerror(errno));
		NetHead->ERRNO1=-194;
		goto errret;
	}
	fgets(logrec.UID,sizeof(logrec.UID),fd);
	TRIM(logrec.UID);
	ShowLog(2,"REGISTER open %s",tmp);
	fclose(fd);
//����ʽ��login��ͬ
	cp=tmp1;
	cp+=sprintf(cp,"%s|%s|", logrec.DEVID,logrec.UID);
	cp+=sprintf(cp,"%s|",rsecstrfmt(tmp,now_sec(),YEAR_TO_SEC));

	NetHead->data=tmp1;
	NetHead->PKG_LEN=strlen(NetHead->data);
	NetHead->ERRNO1=0;
	NetHead->ERRNO2=0;
	NetHead->O_NODE=LocalAddr(conn->Socket,tmp);
	NetHead->O_NODE=htonl(NetHead->O_NODE);
	NetHead->para[3]=0;
//showhex(5,tmp,(char *)&NetHead->O_NODE,4);
    	SendPack(conn,NetHead);
	return -1;//�߳�ȥ��
    } //δע��ͻ���ע�����

/* decode label */
	ret=a64_byte(tmp1,logrec.LABEL);
	enigma2_decrypt(&egm,tmp1,ret);
	tmp1[ret]=0;
	strcpy(logrec.LABEL,tmp1);

ShowLog(5,"LABEL=%s,tmp1=%s,key=%s",logrec.LABEL,tmp1,key);
	freedw(&dw);
//�����ݿ��ǩ������Ӧ�����ӳ�  
	up->poolno=get_dbpool_no(logrec.LABEL);
	if(up->poolno<0) {
		sprintf(tmp1,"�Ƿ���DBLABEL %s",logrec.LABEL);
		NetHead->ERRNO1=-196;
		goto errret;
	}
//�򿪲���ȡ���ݿ��� 
	ret=get_DB_connect(&up->SQL_Connect,up->poolno);
	if(ret) {
		sprintf(tmp1,"���ݿ��ݲ����ã��Ժ�������");
		NetHead->ERRNO1=-200;
		goto errret;
	}
//���ݿ��Ѿ��򿪣������û��Զ������֤����  
        strcpy(gp->ctx.DEVID,gp->devid);
	*logret.DBLABEL=0;
        if(!NetHead->PKG_REC_NUM) {
            ret=ctx_login(up);
            if(ret>0) {
		str_n64a(1,(u_int *)&gp->ctx.ctx_id,logret.DBLABEL);
            	if(ret>1) ShowLog(5,"%s:DEVID=%s,%d",__FUNCTION__,gp->ctx.DEVID,ret);
	    }
        } else if(NetHead->PKG_REC_NUM!=-1) {
        CTX_stu *ctxp=get_ctx(NetHead->PKG_REC_NUM);
                if(!ctxp||strcmp(ctxp->DEVID,gp->ctx.DEVID)) {
                        release_DB_connect(&up->SQL_Connect,up->poolno);
                        sprintf(tmp1,"��Ч��ctx_id %u",NetHead->PKG_REC_NUM);
                        NetHead->ERRNO1=!ctxp?-197:-198;
                        NetHead->ERRNO2=-1;
                        return_error(conn,NetHead,tmp1);
                        ShowLog(1,"%s:%s",__FUNCTION__,tmp1);
                        return 0;
                }
        } else strcpy(logret.DBLABEL,get_DBLABEL(up->poolno));

        strcpy(logret.DBUSER,up->SQL_Connect->UID);
        strcpy(logret.DBOWN,up->SQL_Connect->DBOWN);

        release_DB_connect(&up->SQL_Connect,up->poolno);

        strcpy(logret.DEVID,gp->devid);
        strcpy(logret.UID,gp->operid);
        rsecstrfmt(logret.Logtime,now_sec(),YEAR_TO_SEC);
        net_pack(tmp1,&logret,log_tpl);

	NetHead->data=tmp1;
	NetHead->PKG_LEN=strlen(NetHead->data);
	NetHead->ERRNO1=0;
	NetHead->ERRNO2=0;
	NetHead->O_NODE=LocalAddr(conn->Socket,tmp);
	NetHead->O_NODE=htonl(NetHead->O_NODE);
	NetHead->para[3]=0;
	NetHead->PROTO_NUM=PutEvent(conn,65535 & NetHead->PROTO_NUM);
//showhex(5,tmp,(char *)&NetHead->O_NODE,4);
    	SendPack(conn,NetHead);
	ShowLog(2,"%s:%s Success",__FUNCTION__,tmp1);
	return 1;
}

