create table cron (
	id number(4) primary key,
	time_exp varchar2(256), -- ʱ����ʽ,����Ǵ����ӣ���ʼʱ�䣬���ʱ�䣨�룩����������Ϊ��Զѭ����
	command  varchar2(256), -- ִ�е�����
	note     varchar2(256), -- ע��
	task_type number(1)	-- �������ͣ�0:CRON��1:SimpleTrigger,2:JAVA��,3:SQL���,+10:������
);
