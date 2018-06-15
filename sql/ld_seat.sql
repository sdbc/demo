/ * default with not null */
/* ��һ��������ĸ���Ϊĸ��������ĸ�����������£�

drop table pre_seat_index;
create table pre_seat_index (		-- ĸ������
	Train_no varchar2(6),	-- ȫ���� 
	item	char(1) default '1',-- ��ţ�һ�㣽'1' 
	beg_date date,	-- ��Ч��ʼ���� YYYY-MM-DD 
	end_date date,	-- ��Ч�������� YYYY-MM-DD 
	distribute_time date null, -- ��󷢲�ʱ�� YYYY-MM-DD HH24:MI:SS
	flag    number(1), --* 0:��Ч����������Ч
	note	varchar(20),
	flag	number(1), --* 0:��Ч����������Ч
	primary key (Train_no,item,end_date) 
);
*/
insert into pre_seat_index values('H1','1',to_date('2008-02-01','YYYY-MM-DD'),to_date('2099-12-31','YYYY-MM-DD'),0,null,null);
insert into pre_seat_index values('H2','1',to_date('2008-02-01','YYYY-MM-DD'),to_date('2099-12-31','YYYY-MM-DD'),0,null,null);
insert into pre_seat_index values('H4','1',to_date('2008-02-01','YYYY-MM-DD'),to_date('2008-03-31','YYYY-MM-DD'),0,null,null);
insert into pre_seat_index values('H4A','1',to_date('2008-02-01','YYYY-MM-DD'),to_date('2008-03-31','YYYY-MM-DD'),0,null,null);
insert into pre_seat_index values('H4','1',to_date('2008-04-01','YYYY-MM-DD'),to_date('2099-12-31','YYYY-MM-DD'),0,null,null);
insert into pre_seat_index values('H4A','1',to_date('2008-04-01','YYYY-MM-DD'),to_date('2099-12-31','YYYY-MM-DD'),0,null,null);
insert into pre_seat_index values('H6A','1',to_date('2008-04-01','YYYY-MM-DD'),to_date('2099-12-31','YYYY-MM-DD'),0,null,'H6');
insert into pre_seat_index values('H6B','1',to_date('2008-04-01','YYYY-MM-DD'),to_date('2099-12-31','YYYY-MM-DD'),0,null,'H6');

/*
drop table pre_seat;
create table pre_seat (	--/* ĸ�� 
	Pre_ID varchar2(17), -- ��������pre_seat_index����ֶ�ƴ�ն�����
-- Train_no:itam:beg_date(YYYYMMDD)
	Carno number(2), -- ����ţ����ֳ����Ʊ����ţ�0 
	seat_type number(4),  --ϯ��0���޺� 
	beg_seatno number(3), -- ��ʼϯ��
	end_seatno number(3),-- ����ϯ��
	beg_station number(3), -- �ϳ�վ 
	end_station number(3),  -- ��Զվ 
	shortest_station number(3),  -- ������Զ 
	purpose	number(9),		-- ��; 
	gride   varchar2(50),   -- �г��ȼ����¿յ�ֱ���ؿ�ȡ�
	primary key (Pre_ID,Carno,beg_seatno,beg_station)
)
*/
insert into pre_seat values('H1:1:20080201',1,12,1,120,0,5,0,0,'G04');
insert into pre_seat values('H1:1:20080201',2,12,1,120,0,5,0,0,'G04');
insert into pre_seat values('H1:1:20080201',3,12,1,120,0,1,0,0,'G04');
insert into pre_seat values('H1:1:20080201',3,12,1,120,1,5,0,0,'G04');
insert into pre_seat values('H1:1:20080201',4,12,1,120,0,2,0,0,'G04');
insert into pre_seat values('H1:1:20080201',4,12,1,120,2,5,0,0,'G04');
insert into pre_seat values('H1:1:20080201',5,13,1,50,0,5,0,0,'G04');
insert into pre_seat values('H1:1:20080201',5,13,51,70,0,1,0,0,'G04');
insert into pre_seat values('H1:1:20080201',5,13,51,70,1,5,0,0,'G04');
insert into pre_seat values('H1:1:20080201',5,13,71,100,0,2,0,0,'G04');
insert into pre_seat values('H1:1:20080201',5,13,71,100,2,5,0,0,'G04');
insert into pre_seat values('H2:1:20080201',1,12,1,120,0,5,0,0,'G04');
insert into pre_seat values('H2:1:20080201',2,12,1,120,0,5,0,0,'G04');
insert into pre_seat values('H2:1:20080201',3,12,1,120,0,1,0,0,'G04');
insert into pre_seat values('H2:1:20080201',3,12,1,120,1,5,0,0,'G04');
insert into pre_seat values('H2:1:20080201',4,12,1,120,0,2,0,0,'G04');
insert into pre_seat values('H2:1:20080201',4,12,1,120,2,5,0,0,'G04');
insert into pre_seat values('H2:1:20080201',5,13,1,50,0,5,0,0,'G04');
insert into pre_seat values('H2:1:20080201',5,13,51,70,0,1,0,0,'G04');
insert into pre_seat values('H2:1:20080201',5,13,51,70,1,5,0,0,'G04');
insert into pre_seat values('H2:1:20080201',5,13,71,100,0,2,0,0,'G04');
insert into pre_seat values('H2:1:20080201',5,13,71,100,2,5,0,0,'G04');
insert into pre_seat values('H4:1:20080201',1,12,1,120,0,6,0,0,'G04');
insert into pre_seat values('H4:1:20080201',2,12,1,120,0,2,0,0,'G04');
insert into pre_seat values('H4:1:20080201',2,12,1,120,2,6,0,0,'G04');
insert into pre_seat values('H4:1:20080201',3,12,1,120,0,3,0,0,'G04');
insert into pre_seat values('H4:1:20080201',3,12,1,120,3,6,0,0,'G04');
insert into pre_seat values('H4:1:20080201',4,12,1,120,0,1,0,0,'G04');
insert into pre_seat values('H4:1:20080201',4,12,1,120,1,6,0,0,'G04');
insert into pre_seat values('H4:1:20080201',5,20,1,20,0,6,0,0,'G04');
insert into pre_seat values('H4:1:20080201',5,20,21,22,0,1,0,0,'G04');
insert into pre_seat values('H4:1:20080201',5,20,21,22,1,6,0,0,'G04');
insert into pre_seat values('H4:1:20080201',6,13,1,20,0,6,0,0,'G04');
insert into pre_seat values('H4:1:20080201',6,13,21,30,0,1,0,0,'G04');
insert into pre_seat values('H4:1:20080201',6,13,21,30,1,6,0,0,'G04');
insert into pre_seat values('H4:1:20080201',6,13,31,60,0,2,0,0,'G04');
insert into pre_seat values('H4:1:20080201',6,13,31,60,2,6,0,0,'G04');
insert into pre_seat values('H4:1:20080201',6,13,61,80,0,3,0,0,'G04');
insert into pre_seat values('H4:1:20080201',6,13,61,80,3,6,0,0,'G04');
insert into pre_seat values('H4:1:20080201',6,13,81,100,0,6,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',1,12,1,120,0,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',2,12,1,120,0,2,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',2,12,1,120,2,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',3,12,1,120,0,3,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',3,12,1,120,3,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',4,12,1,120,0,1,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',4,12,1,120,1,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',5,20,1,20,0,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',5,20,21,22,0,1,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',5,20,21,22,1,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',6,13,1,20,0,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',6,13,21,30,0,1,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',6,13,21,30,1,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',6,13,31,60,0,2,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',6,13,31,60,2,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',6,13,61,80,0,3,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',6,13,81,100,0,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080201',6,13,61,80,3,5,0,0,'G04');

insert into pre_seat values('H6A:1:20080401',1,12,1,120,0,5,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',2,12,1,120,0,1,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',2,12,1,120,1,5,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',3,12,1,120,0,2,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',3,12,1,120,2,5,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',4,12,1,120,0,5,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',5,20,1,20,0,5,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',5,20,21,22,0,2,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',5,20,21,22,2,5,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',6,13,1,50,0,5,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',6,13,51,60,0,1,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',6,13,51,60,1,5,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',6,13,61,80,0,2,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',6,13,61,80,2,5,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',6,13,81,100,0,3,0,0,'G04');
insert into pre_seat values('H6A:1:20080401',6,13,81,100,3,5,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',1,12,1,120,0,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',2,12,1,120,0,2,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',2,12,1,120,2,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',3,12,1,120,0,3,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',3,12,1,120,3,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',4,12,1,120,0,1,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',4,12,1,120,1,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',5,20,1,10,0,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',5,20,11,20,0,1,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',5,20,11,20,1,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',5,20,21,22,0,3,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',5,20,21,22,3,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',6,13,1,20,0,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',6,13,21,50,0,1,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',6,13,21,50,1,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',6,13,51,60,0,2,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',6,13,51,60,2,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',6,13,61,80,0,3,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',6,13,61,80,3,6,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',6,13,81,100,0,4,0,0,'G04');
insert into pre_seat values('H6B:1:20080401',6,13,81,100,4,6,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',1,12,1,120,0,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',2,12,1,120,0,2,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',2,12,1,120,2,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',3,12,1,120,0,3,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',3,12,1,120,3,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',4,12,1,120,0,1,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',4,12,1,120,1,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',5,20,1,20,0,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',5,20,21,22,0,1,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',5,20,21,22,1,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',6,13,1,20,0,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',6,13,21,30,0,1,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',6,13,21,30,1,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',6,13,31,60,0,2,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',6,13,31,60,2,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',6,13,61,80,0,3,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',6,13,61,80,3,5,0,0,'G04');
insert into pre_seat values('H4A:1:20080401',6,13,81,100,0,5,0,0,'G04');

insert into pre_seat values('H4:1:20080401',1,12,1,120,0,6,0,0,'G04');
insert into pre_seat values('H4:1:20080401',2,12,1,120,0,2,0,0,'G04');
insert into pre_seat values('H4:1:20080401',2,12,1,120,2,6,0,0,'G04');
insert into pre_seat values('H4:1:20080401',3,12,1,120,0,3,0,0,'G04');
insert into pre_seat values('H4:1:20080401',3,12,1,120,3,6,0,0,'G04');
insert into pre_seat values('H4:1:20080401',4,12,1,120,0,1,0,0,'G04');
insert into pre_seat values('H4:1:20080401',4,12,1,120,1,6,0,0,'G04');
insert into pre_seat values('H4:1:20080401',5,20,1,20,0,6,0,0,'G04');
insert into pre_seat values('H4:1:20080401',5,20,21,22,0,1,0,0,'G04');
insert into pre_seat values('H4:1:20080401',5,20,21,22,1,6,0,0,'G04');
insert into pre_seat values('H4:1:20080401',6,13,1,20,0,6,0,0,'G04');
insert into pre_seat values('H4:1:20080401',6,13,21,30,0,1,0,0,'G04');
insert into pre_seat values('H4:1:20080401',6,13,21,30,1,6,0,0,'G04');
insert into pre_seat values('H4:1:20080401',6,13,31,60,0,2,0,0,'G04');
insert into pre_seat values('H4:1:20080401',6,13,31,60,2,6,0,0,'G04');
insert into pre_seat values('H4:1:20080401',6,13,61,80,0,3,0,0,'G04');
insert into pre_seat values('H4:1:20080401',6,13,81,100,0,6,0,0,'G04');
insert into pre_seat values('H4:1:20080401',6,13,61,80,3,6,0,0,'G04');

--create table seat (		/* ϯλ�� */
--	start_date	date,	/* ʼ������ YYYY-MM-DD*/
--	beg_station number(3),	/* �ϳ�վ */
--	Train_no varchar2(20),	/* ʼ������ */
--	Train_Carno number(2), /* ����ţ����ֳ����Ʊ����ţ�0 */
--	seat_type number(4),	/*ϯ��0���޺�*/
--	seat_no   number(3),	/* ϯλ�� */
--	end_station number(3),	/* ��ǰ����Զվ���ۺ��Ϊ�³�վ */
--	shortest_station number(3),	/* ������Զ */
--	purpose	number(9),		/* ��; */
--	flag	number(1),	/* ��־��0����,1ռ��,2����,-1���� */
--	used_dev varchar2(16), /* �������ն� */
--	used_uid varchar2(16), /* �������� */
--	used_time date ,		/* ������ʱ�� YYYY-MM-DD HH24:MI:SS */
--	primary key(start_date,beg_station,Train_no,Train_carno,
--			seat_type,seat_no)
--);

-- create table seat_remant ( 	/* ������ */
-- 	start_date	date,	/* ʼ������ */
-- 	beg_station number(3),	/* �ϳ�վ */
-- 	Train_no varchar2(20),		/* ʼ������ */
-- 	Train_Carno number(2), /* ����ţ����ֳ����Ʊ����ţ�0 */
-- 	seat_type number(4),	/*ϯ��0���޺�*/
-- 	purpose	number(9),		/* ��; */
-- 	end_station number(3),	/* ��Զվ */
-- 	amoung	number(3),
-- 	primary key(start_date,beg_station,Train_no,Train_carno,
-- 		seat_type,purpose,end_station)
-- ) ;
	
