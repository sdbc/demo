#ifndef THREAD_H
#define THREAD_H

#include "pthread.h"
#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif

//thr_ctx_siz:�߳�context���ߴ�
//thr_entry:�߳̽���ʱ����Ĺ���
//thr_exit:�߳��˳�������Ĺ���
//callback:�̶߳��м���ʱ�Ĺ���
int thread_init(int (*callback)(void *thr_ctx,int n,void *user_ctx),int thr_ctx_siz,int(*thr_entry)(void **),int(*thr_exit)(void **));
int thread_start();
void thread_finish();
int thread_end();
//val������malloc��
void put_to_thread(int num,void *user_ctx);

#ifdef __cplusplus
}
#endif

#endif
