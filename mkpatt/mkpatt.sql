/* ģ���������ã����޸���Ϣ */
drop table PATTERN_COL ;
create table PATTERN_COL (
        TAB_NAME                VARCHAR2(48), -- ����
        COL_NAME                VARCHAR2(48), -- �������ɸ���ROWID�� 
        COL_TYPE                VARCHAR2(12), -- �����ͣ�ΪSDBC���ͣ�DELETE��ʾ���в�����ģ�塣
        COL_LEN                 NUMBER(5),	  -- �г��ȣ���SDBCģ��Ҫ��
        COL_FORMAT              VARCHAR2(36), -- �и�ʽ
        PSEUDO_NAME             varchar2(24), -- α����
        primary key (TAB_NAME,COL_NAME)
);

