/*************************************************
 * һ��ENIGMA������㷨
 * һ��ת�֣�һ��������
 * 256����λ,�ɽ����ֽڵĶ����Ƽ���/����
 * ��ռ�:256! �� 255 �� 253 ....... ��3     
 * ��Ҫ����ͨ��ϵͳ����
 *************************************************/
#include <sys/types.h>
#ifndef ENIGMA_H
#define ENIGMA_H

#define ROTORSZ 256

typedef signed char ENIGMA[3][ROTORSZ];
typedef struct {
	ENIGMA t;
	ENIGMA r;
	u_int crc;
} ENIGMA2;

#ifdef __cplusplus
extern "C" {
#endif
/* �Ľ��� ENIGMA ���� ���г��ȣ�64K */
void enigma_init(ENIGMA t,const char *bin_key,int len);		//��ʼ��
void enigma(ENIGMA t,char *buf,int len);			//����/����
//�����ת��ת�ֻ�����ȫ�������б�������������������
void enigma_encrypt(ENIGMA t,char *string,int len);		//����
void enigma_decrypt(ENIGMA t,char *string,int len);		//���� 

/* ���ܺ����룬������϶��ּ����ֶ� */
void enigma_rev(ENIGMA t,char *buf,int len);
/* ���ź���ܣ����ڽ��� */
void rev_enigma(ENIGMA t,char *buf,int len);

/* ��ǿ��ENIGMA������ȫ�������������������ĵĶ�Ӧ��ϵ */
void enigma2_init(ENIGMA2 *ep,const char *bin_key,int len);	//��ʼ��
void enigma2_encrypt(ENIGMA2 *ep,char *buf,int len);		//����
void enigma2_decrypt(ENIGMA2 *ep,char *buf,int len);		//���� 

#ifdef __cplusplus
}
#endif

#endif
