#include <datejul.h>
main()
{
INT64 t;
char date[30];

      t=rstrsecfmt("2008-02-28_23:50:00","YYYY-MM-DD_HH24:MI:SS");//t����1899-12-31 00��00��00 ��ʼ��ָ�����ڵ�������
      t+=1800;
      rsecstrfmt(date,t,"YYYY-MM-DD_HH24:MI:SS");
      printf("%s\n",date); 
}
