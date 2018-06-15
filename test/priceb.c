#include <stdio.h>
#include <sdbc.h>
#include <ctype.h>
//�����Զ�ݼ�
typedef struct {
    int D;
    int N;
    int K;
} fall_rate1;
 
fall_rate1 fall_tab[]= {//��Զ�ݼ�
    {0,0,0},
    {201,1,20},//��201��500����ݼ���Ϊ10%�����統ǰΪ300���200+(300-200)(1-10%)==290==300*(1-10%)+20�������20������������
    {501,2,70},
    {1001,3,170},
    {1501,4,320},
    {2501,5,570}
};
static int  FALLNUM=(sizeof(fall_tab)/sizeof(fall_rate1));
//��������̷ֶα�
static int dist_segmt[]={20,30,40,50,60,
             70,80,90,100,110,
            120,130,140,150,160,
            170,180,190,200,220,
            240,260,280,300,320,
            340,360,380,400,430,
            460,490,520,550,580,
            610,640,670,700,740,
            780,820,860,900,940,
            980,1020,1060,1100,1150,
            1200,1250,1300,1350,1400,
            1450,1500,1550,1600,1660,
            1720,1780,1840,1900,1960,
            2020,2080,2140,2200,2270,
            2340,2410,2480,2550,2620,
            2690,2760,2830,2900,2980,
            3060,3140,3220,3300,3380,
            3460,3540,3620,3700,3790,
            3880,3970,4060,4150,4240,
            4330,4420,4510,4600,4700,
            4800,4900,5000,5100,5200,
            5300,5400,5500,5600,5700,
            5800,5900,6000};
 
 
float jflc(int distance) //����Ʒ����
{
int *ip,i,v;
 
    if(distance>6000) return (float)(3595+100*((distance-6001)/100));
    for(i=0,ip=dist_segmt;*ip < distance;ip++) i++;
    if(i>0) v=((ip[-1]+*ip)/2);//�涨��distance�����������ֵ�����磺distance��51,��(50+60)/2=55����
    else v=dist_segmt[0];
    for(i=0;i<FALLNUM-1;i++)
        if(fall_tab[i+1].D>v) break;
//printf("%d->%d:%d,%d,%d\n",v,i,
//   fall_tab[i].D, fall_tab[i].N, fall_tab[i].K);
//�����Զ�ݼ�
    return (float)(fround((v*10-v*fall_tab[i].N)/10.0,5,0)+fall_tab[i].K);
}
 
typedef struct {
    double YZ_fee;//Ӳ��
    double YZ_half_fee;
    double RZ_fee;//����
    double RZ_half_fee;
    double K;//�ӿ�
    double T;//�ؿ�
    double YWS_fee;//Ӳ����
    double YWS_half_fee;
    double YWZ_fee;//Ӳ����
    double YWZ_half_fee;
    double YWX_fee;//Ӳ����
    double YWX_half_fee;
    double RWS_fee;//������
    double RWS_half_fee;
    double RWX_fee;//������
    double RWX_half_fee;
    double G2S_fee; //���˸߰���
    double G2X_fee; //���˸߰���
    double acc;//�յ���
} ticket_fee;
 
#define BASE_FEE 0.05861
 
/* ��󣬼�����Ʊ�� */
//#define RPF(fee) ((fee)>5.0?1.0:0.5)//1Ԫ����.5Ԫ��
//����Ʊ�ۣ���Ʊ�ֻ���Ʊ�۱��ؼƷ����
//fac:����=0,�¿յ�=5, �ۿ�һ��=4,�ۿ۶���=3
 
float  base_fee(int distance,ticket_fee *fee_p,int fac)
{
double tmp,YZ_base;
float lc;
double insum;
double r=(10.0+fac)/10.0;
    lc=jflc(distance); //����Ʒ����
    tmp=YZ_base=BASE_FEE * lc; //���Ʊ������YZ_base
    insum=fround(tmp*0.02,4,1);//���շ�
    fee_p->YZ_fee=fround(fround(tmp+insum,5,0)*r,5,0);
    //fee_p->YZ_half_fee=0.5*fee_p->YZ_fee;
//����Ʊ��
    //fee_p->YZ_fee+=RPF(fee_p->YZ_fee);
    //fee_p->YZ_half_fee+=RPF(fee_p->YZ_half_fee);
    if(lc>200) { //Ӳ��+��վ�յ���
        fee_p->YZ_fee+=1.0;
        //fee_p->YZ_half_fee+=1.0;
    }
//��������
    fee_p->RZ_fee=fround(r*fround(2.0*tmp+insum,5,0),5,0);
    //fee_p->RZ_half_fee=0.5*fee_p->RZ_fee;
    //fee_p->RZ_fee+=RPF(fee_p->RZ_fee);
    //fee_p->RZ_half_fee+=RPF(fee_p->RZ_half_fee);
 
//����յ���
    fee_p->acc=fround(tmp*0.25,5,0);
    if(fee_p->acc<1.0) fee_p->acc=1.0;
    fee_p->acc=fround(r*fee_p->acc,5,0);
 
// ����ӿ�
    fee_p->K=fround(tmp*0.2,5,0);
    if(fee_p->K<1.0) fee_p->K=1.0;
    fee_p->K=fround(r*fee_p->K,5,0);
    //fee_p->T=2*fee_p->K;
//����Ӳ��
    tmp=lc>=380?tmp:BASE_FEE*380;//400KM���ڵļƷ����
    fee_p->YWS_fee=fround(r*fround(tmp*1.1,5,0),5,0);
    //fee_p->YWS_half_fee=0.5*fee_p->YWS_fee;
//+���̶�Ʊ��
    //fee_p->YWS_fee += 10.0;
    //fee_p->YWS_half_fee += 10.0;
 
    fee_p->YWZ_fee=fround(r*fround(tmp*1.2,5,0),5,0);
    //fee_p->YWZ_half_fee=0.5*fee_p->YWZ_fee;
//+���̶�Ʊ��
    //fee_p->YWZ_fee += 10.0;
    //fee_p->YWZ_half_fee += 10.0;
 
    fee_p->YWX_fee=fround(r*fround(tmp*1.3,5,0),5,0);
    //fee_p->YWX_half_fee=0.5*fee_p->YWX_fee;
//+���̶�Ʊ��
    //fee_p->YWX_fee += 10.0;
    //fee_p->YWX_half_fee += 10.0;
//��������
    fee_p->RWS_fee=fround(r*fround(tmp*1.75,5,0),5,0);
    //fee_p->RWS_half_fee=0.5*fee_p->RWS_fee;
//������˸߰�
    fee_p->G2S_fee=fround(fee_p->RWS_fee*1.80,5,0);
//+���̶�Ʊ��
    //fee_p->RWS_fee += 10.0;
    //fee_p->RWS_half_fee += 10.0;
    fee_p->RWX_fee=fround(r*fround(tmp*1.95,5,0),5,0);
    //fee_p->RWX_half_fee=0.5*fee_p->RWX_fee;
    fee_p->G2X_fee=fround(fee_p->RWX_fee*1.80,5,0);
//+���̶�Ʊ��
    //fee_p->RWX_fee += 10.0;
    //fee_p->RWX_half_fee += 10.0;
    return lc;
}
 
 
main(int argc,char *argv[])
{
ticket_fee tfee;
float lc;
char buf[100];
int distance,f=0;
 
    if(argc>1) f=atoi(argv[1]);
    while(!ferror(stdin)) {
        if(isatty(0)) {
            printf("Input distabce:");
            fflush(stdout);
        }
        fgets(buf,sizeof(buf),stdin);
        if(feof(stdin)) break;
        if(!isdigit(*buf)) continue;
        lc=base_fee(atoi(buf),&tfee,f);//���Ĳ���������=0,�¿յ�=5, �ۿ�һ��=4,�ۿ۶���=3
        printf("�Ʒ����  Ӳȫ  ��ȫ   �տ�  "
            "Ӳ����ȫ  Ӳ����ȫ  Ӳ����ȫ  ������ȫ  ������ȫ �յ��� �߶���ȫ  �߶���ȫ \n");
        printf("%.1lf\t%4.2lf\t%4.2lf"
            "\t%4.2lf"
            "\t%.2lf\t%.2lf\t%8.2lf\t%.2lf\t%.2lf"          
            "\t%.2lf\t%.2lf\t%.2lf\n",
            lc,tfee.YZ_fee,
            tfee.RZ_fee,
            tfee.K,
            tfee.YWS_fee,
            tfee.YWZ_fee,
            tfee.YWX_fee,
            tfee.RWS_fee,
            tfee.RWX_fee,
            tfee.acc,
            tfee.G2S_fee,tfee.G2X_fee);
 
    }
 
    return 0;
}
