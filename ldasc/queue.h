#ifndef QUEUE_H
#define QUEUE_H

//#define type_size(data_type) sizeof(data_type),#data_type 

typedef struct {
	int next;
	int num;
	void *val;
} node_stu;


#ifdef __cplusplus
extern "C" {
#endif

//��ʼ��org_buf������elements,ʧ�ܷ���-1
int  queue_init(node_stu **org_buf,int elements);
void queue_free(node_stu **org_buf);
//�黹�ڵ�,nΪ�ڵ���org_buf���±�
void queue_release(node_stu *org_buf,int n,int *queue);
node_stu * queue_get(node_stu *org_buf,int *queue);

#ifdef __cplusplus
}
#endif

#endif
