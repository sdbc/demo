/* tuxedo �豸��֤�� 
   �����豸���������ﱸ�� */
create table tuxdev (
	devid varchar2(16) primary key,  --  ���ں�
	devname varchar2(32) null, --�豸����
	devtype number(4), -- 1:��Ʊ�ն�,2�����նˣ�4�����նˡ�
	devca	varchar2(53) null, -- �豸ָ�� ��Ӧ���й�����Կ?
	unit     number(9),  -- ������λ����
	atime	number(3),-- �װ࿪��ʱ�䡡0-719������,0������,��AFC�����۵��
	ptime number(4), --��ҹ�࿪��ʱ�䡡720-1440������
	perm   varchar2(32),	-- Ȩ��
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

/**********************************************************
 * tuxedo�������Ŀͻ��������ļ����ͻ���¼ʱ����һ����¼
 * �������ʱ����clientIDȡ��������֤��Ȩ��
 * logoutʱɾ���ü�¼
 *********************************************************/

drop table tuxcontex;
create table tuxcontex (
	clid   number(18) primary key,   -- tuxedo clientID
	devid varchar2(16),     -- ���ں�
	userid varchar2(16),    -- �û�ID
	username varchar2(32),    -- �û���
	stat_date date,		-- ͳ������
	shift_code number(2),	-- ��δ��룬-1:û�а��
	devtype number(4),	-- �������ͣ�1=��Ʊ����
	unit     number(9),  -- ������λ����
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
create index del_idx on tuxcontex(last_oper);

create table tuxuser (
	userid varchar2(16) primary key,
	username varchar(32) null,
	password varchar(16) null,
	unit     number(9),  -- ������λ����
	last_log date null, -- ����¼ʱ��
	flag	number(1)  -- 0:��Ч
);
