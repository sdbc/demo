/* default with not null */

drop table sell;
create table sell (
	devid		varchar2(16),	/* ���ں� */
	ticket_no	char(8),	/* Ʊ�� */
	stat_date	date,		/* ͳ������  */
	shift_code	number(2),	/* ��δ��룽(ҹ�ࣽ0-4���װࣽ5-9) */
	shift_flg 	char(2),              -- ���ABCDR��BY
	sell_time	date,		/* ��Ʊʱ�� YYYY-MM-DD HH24:MI:SS*/
	stat		number(1),	/* ״̬��0������-1���ϣ�1��Ʊ,2��Ʊ */
	userid		varchar2(16),	/* ����ԱID */
	username	varchar2(32),	/* ����Ա�� */
	on_time		date,		/* �ϳ�ʱ�� YYYY-MM-DD HH24:MI */
	off_time	date,		/* �³�ʱ�� YYYY-MM-DD HH24:MI */
	valid_days   number(2),	/* ��Ч���� */
	start_date	date,		/* ʼ�����ڣ���Ʊ�� */
	train_no	varchar2(12),	/* ȫ���� */
	run_train	varchar2(6),	/* ���г��� */
	carno		number(2),	/* ����� */
	seat_no		number(3),	/* ϯλ�� */
	seat_type	number(3),	/* ϯ�� */
	beg_station		varchar(4),	/* �ϳ�վ */
	station_name	varchar2(12),	/* �ϳ�վ�� */
	station_off	varchar2(4),	/* �³�վ */
	station_off_name	varchar2(12),	/* �³�վ�� */
	station_end	varchar2(4) null,	/* �յ�վ,ͨƱ */
	station_end_name	varchar2(12) null,	/* �յ�վ��,ͨƱ */
	p_through	varchar2(40) null,	/* ���ɴ��� */
	distance	number(6),	/* ��������� */
	total_distance  number(6),	/* ����� */
	gride		varchar2(50),	/* ���ɴ��� */
	ticket_type	number(2),	/* Ʊ�ִ��� 0ȫ,1��,2ѧ,3��,4��,5̽,6��,7��,8��,9ǩ*/
	money		number(12,2),	/* Ʊ�� */
	addtions_money1   number(8,2),	/* ���ӷ�1 ��Ʊ��*/
	addtions_money2   number(8,2),	/* ���ӷ�2 ��վ�յ���*/
	addtions_money3   number(8,2),	/* ���ӷ�3 ���̶�Ʊ��*/
	addtions_money4   number(8,2),	/* ���ӷ�4 ������*/
	addtions_money5   number(8,2),	/* ���ӷ�5 */
	discount	number(8,3),	/* �ۿ۷� */
	total_fee	number(12,2),	/* �ܼ� */
	total_money	number(12,2),	/* �����ֽ� */
	pay_flag	number(1),	/* �����־�� 
			0:�ֽ�1ˢ�� */
	pay_flag2	number(1),	/* ����ʱ���� 
			0:�ֽ�2�ȸ���3����     */
	pay_id		varchar2(60) null,	/* ����ID */
	tride_id	varchar2(20) null,	/* ����ID */
	barcode		varchar2(256) null,	/* ����  */
	chksum		number(12),	/* У��� */
	syncflg		number(1),	/* ͬ����־��0��ͬ����1δͬ�� */
	note		varchar2(64), /* ��ע���޳˵��յ��ε� */
	primary key(devid,ticket_no,stat_date)
);
-- create index account_idx on sell(devid,stat_date,shift_code);

create table tride_pnr (
	tride_id	varchar2(20) ,
	devid		varchar2(16),
	pnrid		varchar2(16) null,
	tride_time	date,
	ticket_num	number(4),
	money		number(12,2),
	primary key(tride_id,devid)
);
create	index pnr_idx on tride_pnr (pnrid,tride_time desc);

create table pnr (
	pnrid		varchar2(16) primary key,
	pnrname		varchar2(16),
	pnrtype		number(1),		/* ��ͬ�������˻� */
	telphone	varchar2(16) null,
	mobile		varchar2(16) null,
	addr		varchar(40) null,
	zipcode		varchar(16) null,
	email		varchar(40) null,
	principl	varchar2(16) null,	/* ������ */
	pass_type	varchar(10) null, 	/* ֤������ */
	pass_no		varchar(20) null,	/* ֤���� */
	pnr_level	number(2),			/* �ȼ� */
	award		number(9),			/* ���� */
	build_date	date,
	last_time	date null
);


/**************************************************************
 * -- �豸��α�
 * ÿ������ֻ��һ����¼����ʼ����û�м�¼����һ�ε�¼ʱ������
 **************************************************************/
--drop table dev_shift;
create table dev_shift (  
	devid varchar2(16) primary key,
	stat_date date null,		-- ͳ������
	shift_flg char(2),		-- ���ABCDR
	shift_code number(2) null,	-- 0-4ҹ�࣬5-9�װࡣ
	userid varchar2(16) null,	-- ��ҵ��ID,��Ϊδ��¼
	log_time date null,		-- ��¼ʱ��
	beg_no char(8) null		-- Ʊ�����
);
/**************************************************************
 * -- �豸��ʷ��α�
 * ����ʱ��dev_shift����������ʱʹ�á�
 * ������Ʊ�ŷ�Χ
 **************************************************************/
--drop table dev_shift_his;
create table dev_shift_his (  
	devid varchar2(16) ,
	stat_date date ,		-- ͳ������
	shift_flg char(2),		-- ���ABCDR
	shift_code number(2) ,	-- 0-4�װ࣬5-9ҹ�ࡣ
	userid varchar2(16) ,	-- ��ҵ��ID
	username varchar2(16) ,	-- ��ҵ����
	log_time date ,		-- ��¼ʱ��
	logout_time date ,		-- �ǳ�ʱ��
	beg_no char(8),		-- Ʊ�����
	end_no char(8),		-- Ʊ��ֹ��
	bill_num number(5),	-- Ʊ��ʹ����
	primary key (devid,stat_date,shift_code)
);


/*************************************************************
 * ����Ʊ�ݱ�--Ʊ�����ʹ��
 *************************************************************/
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
