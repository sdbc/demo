/* default with not null */

drop table STATION;
create table STATION (
	station_code char(3), 	/* �籨���� */
	station_name varchar2(12),
	beg_date     date, /* ��Ч��ʼ�� YYYY-MM-DD */
	end_date     date, /* ��Ч��ֹ�� YYYY-MM-DD */
	abbreviation varchar2(2) null, /* ��� */
	unit	     varchar2(16) null, 	/* ������ */
	line	     varchar2(8) null,		/* ������,����ǽ���վ��ӹ�վ<0�����վ������������ */
	area	     varchar2(16) null,  /* ���ڳ��� */
	spell	     varchar2(20), 		/* ƴ����,Сд */
	station_type char(2) null,	/* ���ͣ�
	[0]=@��,=A����վ,=B�˽���;[1]=@��,=A������վ,=B�ӹ�վ,=D����ר��,=H�ط���·,=P������·�� */
	statis_code  number(10) null,	/* ͳ���� */
	flag	     number(1),		/* 0:��Ч��������Ч */
	ac_fee     number(2),		/* ��վ�յ��� */
	x	     number(8,3) ,	/* ���� */
	y	     number(8,3), 	/* γ�� */
	primary key (station_code,end_date)
);
create index station_py on station(spell);
create index station_name on station(station_name);

drop table TRAIN;
create table TRAIN (	/* ���α� */
	train_no	varchar2(12),	/* ȫ���� */
	beg_date	date,		/* ��Ч��ʼ�� YYYY-MM-DD */
	end_date	date,		/* ��Ч��ֹ�� YYYY-MM-DD */
	start_station	varchar2(12),	/* ʼ��վ  */
	end_station	varchar2(12),	/* �յ�վ */
	station_num	number(3),	/* ͣվ�� */
	total_distance	number(6),	/* ����� */
	start_time	number(4),	/* ����ʱ�䣬������ */
	iterms	 	number(2),      /* ϯλ���� */
	cycle		varchar2(31),	/* ����ϯλ���� */
	gride		char(5), 	    /* �ȼ����ա��졢�ؿ�� */
	seat_type	varchar(10) null,	/*���е�ϯ��A:Ӳ��,B:����,C:��������,D:һ������,E:Ӳ��,I:����,L:��߰� ,O:������������ ,P:����һ������,S:��������,T:����Ӳ�� */
	price_mode	number(6),	    /* Ʊ�۹��� */
	discount_mode	number(6),	    /* �ۿ۹��� */
	days		number(3) null, /*  ϯλ�������� */
	flag		number(1),	/* 0���ã�1������ */
	owner		varchar2(16) null,  /* ������λ */
	note		varchar2(50) null,		/* ��ע  */
	primary key (train_no,end_date)
);

drop table STOP_STATION;
create table STOP_STATION (
	train_no	varchar2(12),  /*  ȫ���� */
	run_Train_no	varchar2(6),  /* ���г��� */
	sequence	number(3),		/* ˳�� */
	beg_date	date,		/* ��Ч��ʼ���� YYYY-MM-DD*/
	end_date	date,		/* ��Ч�������� YYYY-MM-DD*/
	station_code	varchar2(4),	/* ��վ���� */
	distance	number(6),		/* ��ʼ��վ��� */
	running_time	number(6),		/* ��ʼ��վ���з����� */
	stop_time	number(3),		/* ͣ�������� */
	line	number(4),			/* �����ߺ� */
	area_flg	number(1),		/* 0:���ǵ�����1���ǵ��� */
	over		number(4) null,	/* ��Ա��(��վ�����޺���) */		
	flag		number(1),		/* 0���ã�1��վ */
	primary key  (train_no,sequence,end_date )
);
create index s_station_q on stop_station (Train_no,station_code,end_date);
create index stop_station_idx on stop_station(station_code,run_Train_no,end_date);
	
	
