#define CLIENT
#include "xcom.h"
//�ͻ��ˣ�ץȡ�¼� 
int event_catch(T_Connect *conn,int event_no)
{
	ShowLog(5,"event_catch event_no=%d\n",event_no);
	return event_no;
}
