drop table TRAIN_PRICE;
create table TRAIN_PRICE (     /* ���α� */
    id	 	   number(6),
    start_station char(3), /* ʼ��վ  */
    end_station char(3),   /* �յ�վ */
    beg_date    date,       /* ��Ч��ʼ�� YYYY-MM-DD */
    end_date    date,       /* ��Ч��ֹ�� YYYY-MM-DD */
    yz_fee     number(7),       /* Ӳ���� ȫ�����Խ�Ϊ��λ */
    rz_fee     number(7),       /* ������ */
	rz1_fee		number(7),       /* һ�������� */
	rz2_fee		number(7),       /* ���������� */
    yws_fee    number(7),       /* Ӳ���� */
    ywz_fee    number(7),       /* Ӳ���� */
    ywx_fee    number(7),       /* Ӳ���� */
    rws_fee    number(7),       /* ������ */
    rwx_fee    number(7),       /* ������ */
    g2s_fee    number(7),       /* ���˸߰��� */
    g2x_fee    number(7),       /* ���˸߰��� */
    note        varchar2(50) null,      /* ��ע  */
    primary key (id,start_station,end_station,end_date)
);

 -- select * from TRAIN_PRICE where id=30301 AND start_station='VNP' and end_station='TJP' and end_date >= to_date('2009-0731','YYYY-MM-DD');
