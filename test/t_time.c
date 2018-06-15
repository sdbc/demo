#include <datejul.h>
/********************************************************************************************
 * �ж��ڣ�Ӣ�Thanksgiving Day���������ͼ��ô��еĽ��գ����������������
 * ԭ����Ϊ�˸�л�������ĺ��ճɡ�����������1941���𣬸ж�������
 * ÿ��11�µĵ��ĸ������ģ�������һ�����ݼ����죻�����ô��������ĸж���ʱ�䲻ͬ��
 * 10�µڶ�������һ�����й��Ĵ���һ��������һ�죬��ǧ��������ǲ��ܶ�æ����Ҫ���Լ��ļ����žۡ�
 *********************************************************************************************/
//�����ж���
int th_a(int year)
{
short ymd[3];
int day,d;
	ymd[0]=year;
	ymd[1]=11;
	ymd[2]=1;
	day=rymdjul(ymd);
	if(day < 0) return -1;
	d=4-day%7;
	if(d<0) d+=7;
	day+=d; //��һ������
	return day+21;
}


//���ô�ж���
int th_c(int year)
{
short ymd[3];
int day,d;
	ymd[0]=year;
	ymd[1]=10;
	ymd[2]=1;
	day=rymdjul(ymd);
	if(day < 0) return -1;
	d=1-day%7;
	if(d<0) d+=7;
	day+=d; //��һ����һ
	return day+7;
}
//ÿ�ܶ�����8:00 - 12:00�л
main(int argc,char *argv[])
{
INT4 now,today;
int w,m;
char buf[256];
char a[30],c[30];
	tzset();
	printf("ÿ�ܶ�����8:00 - 12:00�л\n");
	printf("input YYYY-MM-DD HH24:MI\n");
	while(fgets(buf,sizeof(buf),stdin)) {
		today=rstrfmttojul(buf,"YYYY-MM-DD");	
		now=rstrminfmt(buf,"YYYY-MM-DD HH24:MI");	
		w=today%7;//�������ܼ�
		m=(now-timezone/60)%1440;//�����ǵ��յĵڼ�����	
		if(w!=2 || m<480 || m>720) {//û�л
			printf("û�л\n");
		} else {
			printf("�л\n");
		}

		sscanf(buf,"%d",&w);
		m=th_a(w);
		rjulstr(a,m);
		m=th_c(w);
		rjulstr(c,m);
		printf("%d��һ�������ж����ǣ�%s�����ô�ж�����%s\n",w,a,c);
		printf("input YYYY-MM-DD HH24:MI\n");
	}
	return 0;
}


