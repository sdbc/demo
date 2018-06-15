T_PkgType pre_seat_index_type[] = {		/* ĸ������ */
	{CH_CHAR,13,"Train_no",0,-1},
	{CH_CHAR,2,"item"},	/* ��ţ�һ�㣽'1' */
	{CH_JUL,sizeof(INT4),"beg_date",YEAR_TO_DAY}, /* ��Ч��ʼ���� YYYY-MM-DD */
	{CH_JUL,sizeof(INT4),"end_date",YEAR_TO_DAY},
	{CH_TINY,1,"flag"},	/* 0:��Ч����������Ч*/
	{CH_TIME,sizeof(INT64),"distribute_time",YEAR_TO_SEC}, /* ��󷢲�ʱ�� YYYY-MM-DD HH24:MI:SS*/
	{CH_CHAR,21,"note"},
	{-1,0,0}
};
T_PkgType pre_seat_type[] = {		/* ĸ�� */
	{CH_CHAR,24,"Pre_ID",0,-1},
	{CH_TINY,1,"Carno"},	/* ����ţ����ֳ����Ʊ����ţ�0 */
	{CH_SHORT,sizeof(short),"seat_type"},	/*ϯ��0���޺� */
	{CH_SHORT,sizeof(short),"beg_seatno"},
	{CH_SHORT,sizeof(short),"end_seatno"},
	{CH_SHORT,sizeof(short),"beg_station"},	/* �ϳ�վ */
	{CH_SHORT,sizeof(short),"end_station"},	/* ��Զվ */
	{CH_SHORT,sizeof(short),"shortest_station"},	/* ������Զ */
	{CH_INT,sizeof(int),"purpose"},	/* ��; */
	{CH_CHAR,51,"gride"},	/* �г��ȼ����¿յ�ֱ���ؿ�� */
	{-1,0,0}
};
T_PkgType seat_type[] = {		/* ϯλ�� */
	{CH_DATE,YEAR_TO_DAY_LEN,"start_date",YEAR_TO_DAY,-1}, /* ʼ������ YYYY-MM-DD*/
	{CH_CHAR,5,"beg_station"},	/* �ϳ�վ */
	{CH_CHAR,13,"Train_no"},	/* ʼ������ */
	{CH_CHAR,7,"run_train"},	/* ���г��� */
	{CH_DATE,YEAR_TO_MIN_LEN,"on_date",YEAR_TO_MIN,-1}, /* �ϳ�ʱ�� YYYY-MM-DD HH24:mi*/
	{CH_TINY,1,"Carno"},	/* ����ţ����ֳ����Ʊ����ţ�0 */
	{CH_SHORT,sizeof(short),"seat_type"},	/*ϯ��0���޺�*/
	{CH_SHORT,sizeof(short),"seat_no"},	/* ϯλ�� */
	{CH_SHORT,sizeof(short),"end_station"},	/* ��ǰ����Զվ���ۺ��Ϊ�³�վ */
	{CH_SHORT,sizeof(short),"shortest_station"},	/* ������Զ */
	{CH_INT,sizeof(int),"purpose"},	/* ��; */
	{CH_CHAR,51,"gride"},   	/* �г��ȼ����¿յ�ֱ���ؿ�� */
	{CH_CHAR,6,"pro"},   	/* ϯλ�����룬������ */
	{CH_TINY,1,"flag"},		/* ��־��0����,1ռ��,2����,-1���� */
	{CH_CHAR,17,"used_dev"},	/* �������ն� */
	{CH_CHAR,17,"used_uid"},	/* �������� */
	{CH_TIME,sizeof(INT64),"used_time",YEAR_TO_SEC}, /* ������ʱ�� YYYY-MM-DD HH24:MI:SS */
	{CH_CHAR,20,"ROWID"},
	{-1,0,0}
};

T_PkgType seat_remant_type[] = { 	/* ������ */
	{CH_DATE,YEAR_TO_DAY_LEN,"on_date",YEAR_TO_DAY,-1}, /* �ϳ����� */
	{CH_CHAR,5,"beg_station"},	/* �ϳ�վ */
	{CH_CHAR,13,"run_train"},	/* ���г��� */
	{CH_TINY,1,"Carno"},	/* ����ţ����ֳ����Ʊ����ţ�0 */
	{CH_SHORT,sizeof(short),"seat_type"},	/*ϯ��0���޺�*/
	{CH_INT,sizeof(int),"purpose"},	/* ��; */
	{CH_SHORT,sizeof(short),"end_station"},	/* ��Զվ */
	{CH_SHORT,sizeof(short),"amoung"},
	{CH_CHAR,20,"ROWID"},
	{-1,0,0}
};
	
