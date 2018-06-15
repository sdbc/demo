drop table ukreg;
create  table ukreg (
	FLD_kid0 number(12),
	FLD_kid1 number(12),
	FLD_uid  varchar2(12), /* USB-Key �ϵı�ʶ�� */
	FLD_userid varchar2(16), /* ���ݿ�Ӧ���û���¼�� */ 
	FLD_devid varchar2(16),	/* ���ں� */
	FLD_puk	varchar2(171),   /* �û�������Կ */
	FLD_flag	number(1),	/* ��ǣ�0����Ч��1����Ч */
	FLD_enddate date		/* ʧЧ���ڣ�YYYY-MM-DD,���Կ� */
);
create unique index uk_pri on ukreg(FLD_kid0,FLD_kid1);
