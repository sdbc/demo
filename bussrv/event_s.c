#define SERVER
#include "xcomd.h"
//Eno���¼��ţ�һ����Э��� 
int event_put(T_Connect *conn,int Eno)
{
	ShowLog(5,"enevt_put Eno=%d",Eno);
	return Eno;
}
