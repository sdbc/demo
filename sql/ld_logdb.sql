/* tuxedo �豸��֤�� 
   �����豸���������ﱸ�� 
drop table tuxdev;
create table tuxdev (
	devid varchar2(16) primary key,  -- 
	devname varchar2(32) null, --�豸����
	devtype number(4), -- 1:��Ʊ�նˡ�
	devca	varchar2(53) null, -- �豸ָ��
	unit     number(12),  -- ������λ����
	atime     number(3),  -- 
	ptime     number(4),  -- 
	perm   varchar2(32),    -- Ȩ��
        purpose1 number(12),    -- ��ʹ�õ���;1
        purpose2 number(12),    -- ��ʹ�õ���;2
        purpose3 number(12),    -- ��ʹ�õ���;3
        purpose4 number(12),    -- ��ʹ�õ���;4
        purpose5 number(12),    -- ��ʹ�õ���;5
        purpose6 number(12),    -- ��ʹ�õ���;6
        purpose7 number(12),    -- ��ʹ�õ���;7
        purpose8 number(12),    -- ��ʹ�õ���;8
	clid   number(18) null,   -- tuxedo clientID
	last_log date null, -- ����¼ʱ�� YYYY-MM-DD HH24:MI:SS
	flag	number(1)  -- 0:��Ч
);
insert into tuxdev values('SPTEST','��ΰΰ��LINUX',3,null,1001,480,1140,'',0,0,0,0,0,0,0,0,0,null,0);
insert into tuxdev values('SP0102003','�����������',3,null,1001,480,1140,'',0,0,0,0,0,0,0,0,0,null,0);
insert into tuxdev values('WEB_JAVA','��Ʊ����WEB����',1,null,1001,480,1140,'',0,0,0,0,0,0,0,0,0,null,0);
insert into tuxdev values('AFC1','�Զ���Ʊ��1',3,null,1001,0,1440,'',0,0,0,0,0,0,0,0,0,null,0);
insert into tuxdev values('SP0102002','��Ʊ�����ն�2',3,null,1001,480,1140,'',0,0,0,0,0,0,0,0,0,null,0);
insert into tuxdev values('JOLTTEST','JOLT�����ն�',3,null,1001,480,1140,'',0,0,0,0,0,0,0,0,0,null,0);
insert into tuxdev values('JOLTTEST1','��ΰΰ��JOLT�����ն�',3,null,1001,480,1140,'',0,0,0,0,0,0,0,0,0,null,0);
*/
/**********************************************************
 * tuxedo�������Ŀͻ��������ļ����ͻ���¼ʱ����һ����¼
 * �������ʱ����clientIDȡ��������֤��Ȩ��
 * logoutʱɾ���ü�¼
drop table tuxcontex;
create table tuxcontex (
	clid   number(18) primary key,   -- tuxedo clientID
	devid varchar2(16),     -- ���ں�
	userid varchar2(16),    -- �û���
	username varchar(32) null,
	stat_date date,         -- ͳ������
        shift_code number(1),   -- ��δ��룬-1:û�а��
	devtype number(4), -- 1:��Ʊ�նˡ�
	unit     number(12),  -- ������λ����
	perm   varchar2(32),	-- Ȩ��
	purpose1 number(12),    -- ��ʹ�õ���;1
	purpose2 number(12),    -- ��ʹ�õ���;2
	purpose3 number(12),    -- ��ʹ�õ���;3
	purpose4 number(12),    -- ��ʹ�õ���;4
	purpose5 number(12),    -- ��ʹ�õ���;5
	purpose6 number(12),    -- ��ʹ�õ���;6
	purpose7 number(12),    -- ��ʹ�õ���;7
	purpose8 number(12),    -- ��ʹ�õ���;8
	last_oper date null    -- ������ʱ��YYYY-MM-DDHH24:MI:SS
);
 *********************************************************/

/*
create table tuxuser (
	userid varchar2(32) primary key,
	username varchar(32) null,
	password varchar(16) null,
	unit     number(12),  -- ������λ����
	last_log date null, -- ����¼ʱ��
	flag	number(1)  -- 0:��Ч
);
*/
insert into tuxuser values('hp','����',null,1001,null,0);
insert into tuxuser values('hww','��άΰ',null,1001,null,0);
insert into tuxuser values('ylh','������',null,1001,null,0);
insert into tuxuser values('llw','������',null,1001,null,0);
insert into tuxuser values('WEBLOGIC','������վ',null,1000,null,0);
insert into tuxuser values('AFCBJN','������վ�Զ���ƱԱ',null,1000,null,0);
