#include <string.h>
#include <crc32.h>
#include <enigma.h>
/*
 *	A one-rotor machine designed along the lines of Enigma
 *	but considerably trivialized.
 */

#define MASK 0377
// ��������ȣ��������ʹ��MD5���Ӵ����ֵ 

/* ���������� */   
void enigma_init(ENIGMA t,const char *bin_key,int len)
{
int		ic, i, k,random,seed;
signed char	temp,*t1,*t2,*t3;

	if(!t || !bin_key) return;
	if(len<=0) len=strlen(bin_key);
	seed=(int)ssh_crc32((unsigned char *)bin_key,len);
	t1=t[0]; t2=t[1]; t3=t[2];

// ����ic����ʼ�������� 
	ic=seed; 
	for(i=0;i<len;i++) ic += bin_key[i];
	ic &= MASK;
// ic����ʼ�������� 
	for(i=0;i<ROTORSZ;i++) {
		t1[i] = (i+ic) & MASK;
		t3[i] = 0;
	}

	if(len>ROTORSZ) {//�����Կ����ROTORSZ,ʹ�����һ����
		bin_key += len-ROTORSZ;
		len = ROTORSZ;
	}
	for(i=0;i<ROTORSZ;i++) {
		seed = (seed<<1) + (signed)bin_key[i%len];
		random = (seed&0X7FFFFFFF) % 65529;	//random(key);
// �������ɾ����������random�����г�ֵ����ɶ�ѡ������㷨 
/* ������������ t1 */
		k = ROTORSZ-1 - i;
		ic = random % (k+1);

		temp = t1[k]; t1[k] = t1[ic]; t1[ic] = temp;
/************************************************************************
 * ���ɷ���� �����ֻҪ���ز�©�İѸ����������������,
 ************************************************************************/
		if(t3[k]!=0) continue;
		ic = (random&MASK) % k;
		while(t3[ic]!=0) ic = (ic+1) % k;
		t3[k] = ic; t3[ic] = k;
	}
/* t2Ϊt1���� */
	for(i=0;i<ROTORSZ;i++)
		t2[t1[i]&MASK] = i;
}

void enigma(ENIGMA t,char *string,int len)
{
register int  n1,n2, k;
signed char *t1,*t2,*t3;

	if(!t || !string || len <= 0) return;
	t1=t[0]; t2=t[1]; t3=t[2];
//��ʼλ�ú���len��T�йأ���֪��T�Ͳ�֪������Ҳ�޷�ͨ�����ġ����ĵĹ�ϵ�ƶ�T��
	n2=t[len&1][(len>>9)&MASK]&MASK;
	n1=t3[(len>>1)&MASK]&MASK;
	for(k=0;k<len;k++,string++){
		*string = t2[(t3[(t1[((signed char)*string+n1)&MASK]+n2)&MASK]-n2)&MASK]-n1;
		if (++n1 >= ROTORSZ) {
			n1 &= MASK;
			if(++n2==ROTORSZ) n2 = 0;
		}
	}
}

void enigma_encrypt(ENIGMA t,char *string,int len)
{
register signed char *p;
register int x,n1;       //x��ת����
signed char *t1,*t2,*t3;
int  n2, k;
//int r=0;

        if(!t || !string || len <= 0) return;
        t1=t[0]; t2=t[1]; t3=t[2];
//��ʼλ�ú���ת������len��T�йأ���֪��T�Ͳ�֪�����ǣ�Ҳ�޷�ͨ�����ġ����ĵĹ�ϵ�ƶ�T��

	n2=t[len&1][(len>>9)&MASK]&MASK;
	n1=t3[(len>>1)&MASK]&MASK;

	p=(signed char *)string;
	for(k=0;k<len;k++,p++){
		x=t1[(*p+n1)&MASK];
		*p = t2[(t3[(x+n2)&MASK]-n2)&MASK]-n1;
		n1 += (x&0x1F) + 1;
		if (n1 >= ROTORSZ) {
			n1 &= MASK;
			if(++n2==ROTORSZ) n2 = 0;
		}
	}
}

void enigma_decrypt(ENIGMA t,char *string,int len)
{
register signed char *p;
register int n1, n2, k,x;
signed char *t1,*t2,*t3;

        if(!t || !string || len <= 0) return;
        t1=t[0]; t2=t[1]; t3=t[2];
//��ʼλ�ú���ת������len��T�йأ���֪��T�Ͳ�֪�����ǣ�Ҳ�޷�ͨ�����ġ����ĵĹ�ϵ�ƶ�T��
	n2=t[len&1][(len>>9)&MASK]&MASK;
	n1=t3[(len>>1)&MASK]&MASK;

	p=(signed char *)string;
	for(k=0;k<len;k++,p++){
		x=t3[(t1[(*p+n1)&MASK]+n2)&MASK]-n2;
		*p = t2[x&MASK]-n1;
		n1 += (x&0x1F)+1;
		if (n1 >= ROTORSZ) {
			n1 &= MASK;
			if(++n2==ROTORSZ) n2 = 0;
		}
	}
}

