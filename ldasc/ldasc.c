#include <stdio.h>
#include <DAU.h>
#include <DAU_json.h>
#include <libgen.h>

#define trans_begin(SQL_Connect)  ___SQL_Transaction__(SQL_Connect,TRANBEGIN)
#define trans_rollback(SQL_Connect)  ___SQL_Transaction__(SQL_Connect,TRANROLLBACK)
#define trans_commit(SQL_Connect)  ___SQL_Transaction__(SQL_Connect,TRANCOMMIT)

int unloadfile(T_SQL_Connect *SQL_Connect,char *tablename,FILE *ifd,FILE *ofd,
		int Pflg,char *where,int buflen,char dlmt)
{
char *p;
DAU _DAU;
int num,ret;
INT64 now;

	ShowLog(5,"unloadfile:entry dlmt=0X%02X",dlmt&255);
	ret=DAU_init(&_DAU,SQL_Connect,tablename,0,0);
	if(ret) {
		ShowLog(1,"unloadfile:DAU_init tabname=%s,ret=%d",tablename,ret);
		return -1;
	}
	now=now_usec();
	ret=DAU_prepare(&_DAU,where);
	if(ret) {
		ShowLog(1,"unloadfile:err=%d,%s",ret,where);
		return ret;
	}
	ret=1;
	num=0;
	while(!DAU_next(&_DAU)) {
	    if(dlmt=='J') {
		JSON_OBJECT json=json_object_new_object();
		DAU_toJSON(&_DAU,json,0);
		fprintf(ofd,"%s\n",json_object_to_json_string(json));	
		json_object_put(json);
	    } else {
		ret=DAU_pkg_pack(&_DAU,where,dlmt);
		fwrite(where,ret,1,ofd);
		fputc('\n',ofd);
	    }
		num++;
	}
	DAU_free(&_DAU);
	ShowLog(2,"unload %s:rows=%d,INTERVAL=%d",tablename,num,INTERVAL(now));
	return 0;
}

int loadfile(T_SQL_Connect *SQL_Connect,char *tablename,FILE *ifd,
	     FILE *ofd,int Pflg,char *buf,int buflen,char dlmt)
{
char *p,tabn[512];
DAU _DAU;
int rows,ret;
int upd,loss;
int num;
INT64 now;

	ShowLog(5,"loadfile:entry dlmt=0X%02X",dlmt&255);
	ret=DAU_init(&_DAU,SQL_Connect,tablename,0,0);//�����ݿ⽨��ģ��
	if(ret) {
		ShowLog(1,"loadfile:DAU_init tabname=%s,ret=%d",tablename,ret);
		return -1;
	}
	upd=loss=0;
	num=0;
	now=now_usec();
	for(rows=0;!ferror(ifd);rows++) {
		fgets(buf,buflen,ifd);
		if(feof(ifd)) break;
		if(dlmt=='J') {
			JSON_OBJECT json=json_tokener_parse(buf);
			if(!json) {
				continue;
			}
//ShowLog(5,"%s:json=%s",__FUNCTION__,buf);
			DAU_fromJSON(&_DAU,json);
			json_object_put(json);
		} else {
			ret=DAU_pkg_dispack(&_DAU,buf,dlmt);
		}
		*buf=0;
		ret=DAU_insert(&_DAU,buf);
		if(ret) {
			if(SQL_Connect->Errno == DUPKEY) {
				if(!Pflg) {//���û��-Pѡ��
					*buf=0;
					ret=update_by_PK(&_DAU,buf);
					if(ret>0) {
						upd+=ret;
					} else if(ret<0) {
						ret=DAU_print_bind(&_DAU,buf);
						ShowLog(1,"loadfile:update err=%d,%s:bind=%s",
							SQL_Connect->Errno,SQL_Connect->ErrMsg,buf);
						loss++;
						break;
					}
				} else {//��-Pѡ�����ʧ��д��ָ���ļ�
					DAU_pkg_pack(&_DAU,buf,dlmt);
					fprintf(ofd,"%s\n",buf);
//10G���������,11G����û��
                                        ret=dummy_update(&_DAU,buf);

					loss++;
				}
			} else {
				DAU_pack(&_DAU,buf);
				fprintf(ofd,"%s\n",buf);
				ret=DAU_print_bind(&_DAU,buf);
				ShowLog(1,"loadfile:bind=%s",buf);
				loss++;
				break;
			}
			rows--;
			continue;
		}
		if(!(++num %10000)) {
			ret=trans_commit(SQL_Connect);
		}
	}
	ShowLog(2,"loadfile:rows=%d,upd=%d,loss=%d,TIMEVAL=%ld",rows,upd,loss,now_usec()-now);
	DAU_free(&_DAU);
	return rows;
}

static char my_showid[200];
#define DEF_BUF_SIZ 4096
main(int argc,char *argv[])
{
int ret,i,len=DEF_BUF_SIZ;
T_SQL_Connect SQL_Connect;
int Pflg=0;
char *tabname=0;
FILE *ifd,*ofd;
INT64 now;
char *myname;
char *where=0;
pthread_t trans_tid;
char *dlmt="|";

	tzset();
	myname=getenv("GBK_FLAG");
	if(myname && *myname) GBK_flag=1;
	myname=sc_basename(argv[0]);
	sprintf(my_showid,"%s:%d",
		myname,getpid());
	Showid=my_showid;
	ifd=0;
	ofd=0;

/*******************************************************************
 *  get Opt
 *******************************************************************/
	for(i=1;i<argc;i++) {
		if(*argv[i]=='-') {
			switch(argv[i][1]) {
			case 'f':	//�����ļ�
				if(argv[i][2]) ret=envcfg(argv[i]+2);
				else {
					i++;
					ret=envcfg(argv[i]);
				}
				continue;
			case 'P':  //��� ���ܼ��صļ�¼
				Pflg=1;
				if(argv[i][2]) {
					ofd=fopen(argv[i]+2,"w");
				} else {
					i++;
					if(i<argc) ofd=fopen(argv[i],"w");
				}
				continue;
			case 'K':  //���ù���buff��Kbyte�� 
				if(argv[i][2]) {
					len=atoi(argv[i]+2);
				} else {
					i++;
					if(i<argc) len=atoi(argv[i]);
				}
			case 'W': // WHERE�Ӿ�
				if(argv[i][2]) {
					where=argv[i]+2;
				} else {
					i++;
					if(i<argc) where=argv[i];
				}
				continue;
			case 'd':  //���÷ָ���
				if(argv[i][2]) {
					dlmt=&argv[i][2];
				} else {
					i++;
					if(i<argc) dlmt=argv[i];
				}
				continue;
					
			default:
				fprintf(stderr,"no know option:%s\n",argv[i]);
 				fprintf(stderr,"Usage:%s -f �����ļ� [-P] ����ļ��� [-Kkbytes] [-W]WHERE �Ӿ� [-d�ָ���]\n",
					argv[0]);
				continue;
			}
		}
		tabname=argv[i];
	}
	if(len < DEF_BUF_SIZ) len=DEF_BUF_SIZ;
char buf[len];
	*buf=0;
	if(where) strcpy(buf,where);
	char *cp=getenv("COL_TO_LOWER");
	if(cp && isdigit(*cp)) col_to_lower=atoi(cp);
	if(!tabname) return 2;
	ret=db_open(&SQL_Connect);
	if(ret) {
		ShowLog(1,"Open Database err=%d.%s",
			SQL_Connect.Errno,
			SQL_Connect.ErrMsg);
		return 1;
	}
ShowLog(5,"DB=%s",SQL_Connect.DBOWN);

	if(!ofd) ofd=stdout;
	ifd=stdin;
	
	if(*dlmt=='\\') switch(dlmt[1]) {
		case 't':
			*dlmt='\t';
			break;
		case 'b':
			*dlmt=' ';
			break;
		case 'n':
			*dlmt='\n';
			break;
		case '0':
		case 0:
			*dlmt='\0';
			break;
		default:
			*dlmt='|';
			break;
	}
	now=now_usec();
//������ҵ����
	if(strncmp(myname,"un",2)) {
int cc;
		ret=loadfile(&SQL_Connect,tabname,ifd,ofd,Pflg,buf,sizeof(buf),*dlmt);
		ShowLog(2,"loadasc:load %d rec's INTERVAL=%d",ret,INTERVAL(now));
	now=now_usec();
		cc=trans_commit(&SQL_Connect);
ShowLog(5,"loadfile:%d commit TIMEVAL=%ld,ret=%d",ret,(int)(now_usec()-now),cc);
	} else ret=unloadfile(&SQL_Connect,tabname,ifd,ofd,Pflg,buf,sizeof(buf),*dlmt);

	now=now_usec();
	ret=___SQL_CloseDatabase__(&SQL_Connect);
ShowLog(5,"Close_Database ret=%d,TIMEVAL=%ld",ret,now_usec()-now);
	if(ofd && ofd != stdout) fclose(ofd);
	return 0;
}
