/**************************************************************************
 * memzip.c ������ ������ 2007.6.13
 * ver 0.0.3.0 ʹ����HASH��֧�ֻ������ڡ�ȡ���˴��ڴ�С�����á�
 * Ϊ���߽���д��һ��ѹ������ѹ���ǻ����ַ����ģ�����deflate�㷨
 * ��ʹѹ�����ˣ�Ҳ�����ܱ䳤������ͨ�Ź��̲����洢���
 * ��ʱû�в��� huffuman�㷨�����ǵ�ÿ��ͨ�Ű�����huffman����ѹ����������ͼ
 * �㷨��Դ��gzip���Ľ�֮�����ڼ�ǿ��Сƥ���Ч�ʣ�������Ϊ����ƥ������ԭ����
 * �����ݱ�ߣ���ʹѹ��������ݱ��Ϊ9/8��
 * �������ñ���ֽ������ƥ���ӡ�
 * ���ԭ���к��б�ǣ���ת�塣
 * ��������ñ䳤ƥ���ӡ���С��ƥ����2�ֽڣ�����4�ֽڡ�
 **************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sc.h>

/* used by comprtok.flgbyte */
#define ESC  0X10		//ת���ַ�
#define MFLG 0X11		//����ַ�
#define MBASE 0x12		//��ƥ�����ַ���MBASE��MBASE+5,�ֱ��ʾ3-8�ֽڵ�ƥ�䳤�ȡ�

#define MAX_DISTANCE 16576

#define HASH_BITS 15
#define HASH_SIZE (unsigned)(1<<HASH_BITS)
#define HASH_MASK (HASH_SIZE-1)
#define NIL (unsigned short)0xFFFF
#define DNIL (((unsigned int)NIL << 16)+NIL)
#define HASH(s,c) ((((unsigned)(s)<<5) ^ (unsigned)(c)) & HASH_MASK)

/* ƥ���ӵ������ṹ */
struct comprtok {
	char flgbyte;	//����ַ�
	char len;	//ƥ�䳤�ȣ���ƥ�䲻ʹ�á���ƥ���ʾ9��264�ֽڵ�ƥ�䡣
/* �䳤��ƥ����롣���3�ֽ�ƥ�䣬distance[0]=0~255,��ʾ1��256�ֽڷ�Χ�ڵ�ƥ�䣬
���ֽ�ƥ��ʱ��distance[0]={0-191},��ʾ1-192��Χ�ڵ�ƥ�䣬һ���ֽڡ����D7D6��1��
��Ҫ�����ֽڣ���ʾ193-MAX_DISTANCE��ƥ����� */
	unsigned char distance[2];
	char *strend;	//ƥ���Ӻ�����ֽ�
	char *strbegin; //���ڿ�ʼ��λ��
	unsigned short head[HASH_SIZE]; //hash head
	unsigned short prev[32768];	//link tab
	unsigned short h;		//HASH key
};

/* deflateԤѹ������ڲ�����
tokp:ƥ������������
strbegin:����ʼ��λ�á�
strstart:��ǰλ�á�
len:strstart֮��ĳ��ȡ�
����ֵ:<0��ʾû��ѹ����>=0��ʾѹ������ƥ����������tokp��
*/
#define IBASE (MBASE-3)
static int deflate(register struct comprtok *tokp,register char *strstart,int len)
{
register char *p,*p1;
int i,gain,g1,j;
register unsigned int pos,b;
int distance,cur;
unsigned short *prev;
	if(len < 3) return -1;
	cur=strstart-tokp->strbegin;
	b=0;
	if(cur>MAX_DISTANCE) b=cur-MAX_DISTANCE;
	gain=-1;
	prev=tokp->prev;
	
	for(pos=tokp->head[tokp->h]; pos != NIL && pos>=b;
		pos=prev[pos]) {
/*
if(pos==tokp->prev[pos]) {
	printf("deflate pos=%d,cur=%d,hash=%d,head=%d\n",
		pos,cur,tokp->h,tokp->head[tokp->h]);
	break;
}
*/
	    p=tokp->strbegin+pos;
	    if(*strstart!=*p || strstart[1]!=p[1] || strstart[2]!=p[2])
			continue;
	    distance=cur-pos;
	    i=3;
	    p+=i;
		for( p1=strstart+i ; i<len && *p==*p1;p++,p1++)
		{
			if(++i == 264) {
				p++,p1++;
				break;
			}
	   	}
/* �������� */
	    g1=i-2;
	    if(i==3) {
		if(distance>256) continue;

	    } else {
		if(distance>192) --g1;
	    	if(i>8) g1--;
	    }
	    j=(distance<i);	//�ص�
	    if(g1>gain) {
		if(i<9) tokp->flgbyte=i+IBASE;
		else {
			tokp->flgbyte=MFLG;
			tokp->len=i-9;
		}
		if(i==3 || distance<193) {
			*tokp->distance=distance-1;
		} else {
			distance -= 193;
			*tokp->distance=(distance>>8) | 0xC0;
			tokp->distance[1]=distance;
		}
		tokp->strend=p1;
		gain=g1;
		if(j && g1>20) break; //�ص��˳�������ѹ��������ߣ��ٶȺ���
		if(i>263) break;
	    }
	}
/*
if(gain > 0) {
	if(tokp->flgbyte==MFLG) i=(tokp->len&255)+9;
	else i=tokp->flgbyte-IBASE;
	if(*tokp->distance<192) distance=*tokp->distance+1;
	else {
		distance=*tokp->distance & 0X3F;
		distance<<=8;
		distance += tokp->distance[1];
		distance += 193;
	}
	if(distance > 128 && distance < 193)
	    fprintf(stderr,"%d<%d,%d>\n",strstart-tokp->strbegin,
		i,distance,i,strstart);
}
*/
	return gain;
}

static void win_slip(struct comprtok *tokp)
{
int i;
register unsigned short *usp,*udp;
	usp=tokp->head;
	for(i=0;i<HASH_SIZE;i++,usp++) {
		if(*usp==NIL) continue;
		if(*usp<16384) *usp=NIL;
		else *usp-=16384;
	}
	udp=tokp->prev;
	usp=udp+16384;
	for(i=0;i<16384;i++,usp++,udp++) {
		if(*usp==NIL || *usp<16384) *udp=NIL;
		else *udp=*usp-16384;
	}
	tokp->strbegin+=16384;
}
/* memzip,��Ҫ��ѹ������
 ��ڲ�����
src����ѹ���Ĵ���
srcLen�������ֽ�����
dest��ѹ����Ĵ�����ڴˣ��䳤�Ȳ�С��srcLen
����ֵ��ѹ����ĳ��ȣ����==srcLen��û��ѹ��
*/
int memzip(char *dest,char *src,int srcLen)
{
register unsigned char *p,*pd,*p0,*pe;
int gain=0,totalgain=0;
unsigned cur,i;
struct comprtok comtok;
	if(!src || !dest) return 0;
    {
	register unsigned *up;
	register unsigned num;
	num=HASH_SIZE>>1;
	up=(unsigned *)comtok.head;
	do *up++ = DNIL; while(--num);
/*
	memset(comtok.head,0xFF,sizeof(comtok.head));
*/
    }
	p=(unsigned char *)src;pd=(unsigned char *)dest;
	comtok.strbegin=src;
	comtok.h=HASH(HASH(*p,p[1]),p[2]);
	comtok.head[comtok.h]=0;
	*comtok.prev=NIL;
	*pd++=*p++;
	p0=(unsigned char *)src+srcLen;
	pe=(unsigned char *)dest+srcLen;
	while(p<p0) {
		if(p<p0-2) comtok.h=HASH(comtok.h,p[2]);
		if((cur=p-(unsigned char *)comtok.strbegin) > 32504) { //���ڻ���16384
//fprintf(stderr,"slip cur=%u\n",cur);

			win_slip(&comtok);
			cur -= 16384;
/*
printf("win_slip cur=%d,strbegin=%d,srcLen=%d,p=%d\n",
cur,comtok.strbegin-src,srcLen,p-src);
*/
		}
		gain=deflate(&comtok,(char *)p,p0-p);
/* ��ǰ�ַ�����HASH�� */
		comtok.prev[cur]=comtok.head[comtok.h];
		comtok.head[comtok.h]=cur;
		if(gain>0) {	//ѹ����
		    p=(unsigned char *)comtok.strend;
			i=p-(unsigned char *)comtok.strbegin;  //�ܹ�HASH������ֽ�
/* ƥ�����������ַ���Ҫ����HASH�� */	
			while(++cur < i) {
				comtok.h=HASH(comtok.h,
				    *((unsigned char *)comtok.strbegin+cur+2));
/*
if(cur & 0XFFFFC000)
	fprintf(stderr,"cur=%u\n",cur);
*/
				comtok.prev[cur]=comtok.head[comtok.h];
				comtok.head[comtok.h]=cur;
			}
		    *pd++ = comtok.flgbyte;
		    if(comtok.flgbyte == MFLG) 	//��ƥ��
			    *pd++ = comtok.len;
		    *pd++ =*comtok.distance;
		    if(comtok.flgbyte!=MBASE && *comtok.distance > 191) {
				*pd++ = comtok.distance[1];	//������
		    }
		    totalgain += gain;
		    continue;
		}
		if(*p>=ESC && *p <(ESC+8))  {
			totalgain--;
			if(pd>pe) //  ��ֹԽ��
			{
				totalgain=-1;
				break;
			}
			*pd++=ESC; //ת��
		}
		if(pd>pe) {	//��ֹԽ��
			totalgain=-1;
			break;
		}
		*pd++ = *p++; //ԭ�ĸ���
	}
	if(totalgain <=0 && p != p0) {
		memcpy(dest,src,srcLen);
		return srcLen;
	}
	return pd-(unsigned char *)dest;
}

/* ��Ҫ�Ľ�ѹ������
��ڲ�����
compr_str������memzipѹ�����ַ�����
t_len:compr_str�ĳ��ȡ�
buf����ѹ��Ĵ���
pkg_len:buf�ĳ��ȣ�Ӧ�þ���ѹ��ǰ�ĳ��ȡ�
����ֵ����ѹ��ĳ���,Ӧ�õ���ѹ��ǰ�ĳ��ȡ�
-1:���ֲ����������������memzipѹ���ġ�
*/
int memunzip(char *compr_str,int t_len,char *buf,int pkg_len)
{
register unsigned char *p,*p1,*p2,*p0;
int n;
int distance;
	if(!compr_str || !buf) return 0;
	if(t_len==pkg_len) {
		if(compr_str != buf)
			memcpy(buf,compr_str,t_len);
		return(t_len);
	}
	p=(unsigned char *)compr_str;
	p1=(unsigned char *)buf;
	*p1++ = *p++;
	p0=(unsigned char *)(compr_str+t_len);
	while(p<p0) {
		if(p1 == p) {
			return pkg_len;
		}
		if((p1-(unsigned char *)buf)>=pkg_len) {
//fprintf(stderr,"���ֲ������󣬽��볬��%d��t_len=%d,possion=%d\n",
//		p1-buf-pkg_len,t_len,p-(unsigned char *)compr_str);
			return -1;
		}
		switch((unsigned char)*p) {
		case MBASE:
		case MBASE+1:
		case MBASE+2:
		case MBASE+3:
		case MBASE+4:
		case MBASE+5:	//��ƥ��
			n=*p-IBASE;
			p++;
			if(n != 3) goto get_distance;
			else { //���ֽ�distance:=0-255;
				distance=(*p++ & 255)+1;
				goto byte3;
			}

		case MFLG:	//��ƥ��
			p++;
			n=(*p&255)+9;
			p++;
get_distance:
			if(*p > 191) {
				distance= *p&0X3F;
				distance <<= 8;
				p++;
				distance += *p&255;
				p++;
				distance += 193;
			} else {
				distance=(*p)+1;
				p++;
			}
byte3:
			p2=p1-distance;
			while(n--) *p1++ = *p2++;
			break;
		case ESC:	//���ת��
			p++;
		default:
			*p1++ = *p++;
			break;	
		}
	}
	return p1-(unsigned char *)buf;
}
