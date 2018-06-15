
drop table seat;
create table seat (		/* ϯλ�� */
	start_date	date,	/* ʼ������ YYYY-MM-DD*/
	beg_station varchar2(4),	/* �ϳ�վ���� */
	Train_no varchar2(12),	/* ȫ���� */
	run_train	varchar2(6),	/* ���г��� */
	on_date	date,		/* �ϳ����� YYYY-MM-DD HH24:MI*/
	Carno number(2),	/* ����ţ����ֳ����Ʊ����ţ�0 */
	seat_type number(2),	/*ϯ��0���޺�*/
	seat_no   number(3),	/* ϯλ�� */
	end_station number(3),	/* ˳�ţ���ǰ����Զվ���ۺ��Ϊ�³�վ */
	shortest_station number(3),	/* ˳�ţ�������Զ */
	purpose	number(9),		/* ��; */
	gride	varchar2(50),   -- �����룬�¿յ�ֱ���ؿ�ȡ�
	pro	char(5) null,	-- ϯλ�����루�������ߵ���
	flag	number(1),	/* ��־��0����,1ռ��,2����,3�ݲ����ã�-1���� */
	used_dev varchar2(16) null, /* �������ն� */
	used_uid varchar2(16) null, /* �������� */
	used_time date  null,		/* ������ʱ�� YYYY-MM-DD HH24:MI:SS */
	primary key(start_date,beg_station,Train_no,Carno,
			seat_no,seat_type)
);
-- create index seat_idx on seat(beg_station,on_date,run_train,end_station);
create index seat_release on seat(flag,used_time); /* �ͷ�ռ�� */

drop table seat_remant;
create table seat_remant ( 	/* ������ */
	on_date	date,			/* �ϳ����� */
	beg_station varchar2(4),	/* �ϳ�վ���� */
	run_train varchar2(12),		/* ���г��� */
	Carno number(2),	/* ����ţ����ֳ����Ʊ����ţ�0 */
	seat_type number(4),	/*ϯ��0���޺�*/
	purpose	number(9),		/* ��; */
	end_station number(3),	/* ��Զվ */
	amoung	number(3), 	/* ���� */
	primary key(on_date,beg_station,run_train,Carno,
		seat_type,purpose,end_station)
);
	
