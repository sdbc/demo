/* ���ļ�����ʱ�ļ���������sell.sql�*/
/**************************************************************
 * -- �豸��α�
 * ÿ������ֻ��һ����¼����ʼ����û�м�¼����һ�ε�¼ʱ������
 **************************************************************/
create table dev_shift (  
	devid varchar2(16) primary key,
	stat_date date null,		-- ͳ������,��Ϊδ��¼
	shift_code number(1) null,	-- 0-4�װ࣬5-9ҹ��
	userid varchar2(16) null,	-- ��ҵ��ID
	log_time date null,		-- ��¼ʱ��
	beg_no char(8) null		-- Ʊ�����
);
/**************************************************************
 * -- �豸��ʷ��α�
 * ����ʱ��dev_shift����������ʱʹ�á�
 * ������Ʊ�ŷ�Χ
 **************************************************************/
create table dev_shift_his (  
	devid varchar2(16) ,
	stat_date date ,		-- ͳ������
	shift_code number(1) ,	-- 0-4�װ࣬5-9ҹ�ࡣ
	userid varchar2(16) ,	-- ��ҵ��ID
	username varchar2(16) ,	-- ��ҵ����
	log_time date ,		-- ��¼ʱ��
	logout_time date ,		-- �ǳ�ʱ��
	beg_no char(8),		-- Ʊ�����
	end_no char(8),		-- Ʊ��ֹ��
	primary key (devid,stat_date,shift_code)
);


/*************************************************************
 * ����Ʊ�ݱ�--Ʊ�����ʹ��
 *************************************************************/
drop table dev_bill;
create table dev_bill (
	devid varchar2(16) primary key,
	bill_num char(8) null,	--��ǰƱ��
	vol0 char(8) null,	-- ���
	vol1 char(8) null,	-- ���
	vol2 char(8) null,	-- ���
	vol3 char(8) null,	-- ���
	vol4 char(8) null,	-- ���
	vol5 char(8) null,	-- ���
	vol6 char(8) null,	-- ���
	vol7 char(8) null,	-- ���
	vol8 char(8) null,	-- ���
	vol9 char(8) null,	-- ���
	vol10 char(8) null,	-- ���
	vol11 char(8) null,	-- ���
	vol12 char(8) null,	-- ���
	volp number(2) default 0
);
