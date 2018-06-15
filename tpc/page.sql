���û�������DBA�Ը��û���Ȩ��
GRANT CREATE ANY CONTEXT TO XXXXX;

�����XXXXX�������������û�����

Ȼ����XXXXX��¼��

CREATE OR REPLACE CONTEXT APP_CTX USING get_data;
---- ���洴����CONTEXT��Ϊ�˴��ݰ󶨱�����

CREATE OR REPLACE PACKAGE get_data
AS
PROCEDURE read (
          p_output     OUT SYS_REFCURSOR
         ,p_tablename   IN VARCHAR2
         ,p_where       IN VARCHAR2
         ,p_page_size   IN NUMBER DEFAULT 0
         ,p_page_idx    IN NUMBER DEFAULT 0
         ,p_values      IN VARCHAR2
         );
END;
/



CREATE OR REPLACE PACKAGE BODY get_data
AS
PROCEDURE read (
          p_output     OUT SYS_REFCURSOR
         ,p_tablename   IN VARCHAR2
         ,p_where       IN VARCHAR2
         ,p_page_size   IN NUMBER DEFAULT 0
         ,p_page_idx    IN NUMBER DEFAULT 0
         ,p_values      IN VARCHAR2
         )
AS
  lv_sql           VARCHAR2(32000);
  lv_where         VARCHAR2(32000) := p_where;
  lv_current_col   VARCHAR2(30);
  lv_col_predicate VARCHAR2(100);
  lv_values        VARCHAR2(32000) := p_values;
  lv_val1          VARCHAR2(1000);
  lv_pos           NUMBER;
  lv_min           NUMBER;
  lv_max           NUMBER;
  lv_cnt           NUMBER :=0;
  lv_data_type     USER_TAB_COLUMNS.DATA_TYPE%TYPE;
  lv_date_format   VARCHAR2(20) := 'YYYYMMDDHH24MISS';
  lv_time_format   VARCHAR2(20) := 'YYYYMMDDHH24MISS';
BEGIN
  WHILE lv_values IS NOT NULL 
  LOOP
      lv_pos := INSTR(lv_values,',');
      IF lv_pos>0 THEN
         lv_val1 := SUBSTR(lv_values,1,lv_pos-1);
         lv_values := SUBSTR(lv_values,lv_pos+1);
      ELSE
         lv_val1 := lv_values;
         lv_values := NULL;
      END IF;
      lv_pos := INSTR(lv_val1,':');
      lv_current_col := TRIM(BOTH '"' FROM SUBSTR(lv_val1,1,lv_pos-1));
      BEGIN
         SELECT data_type INTO lv_data_type FROM USER_TAB_COLUMNS WHERE TABLE_NAME = UPPER(p_tablename) AND COLUMN_NAME=UPPER(lv_current_col);
      EXCEPTION
         WHEN NO_DATA_FOUND THEN
              RAISE_APPLICATION_ERROR(-20001,'invalid column name in parameter:'||lv_current_col);
      END;

      lv_cnt := lv_cnt+1;
      DBMS_SESSION.SET_CONTEXT('APP_CTX','$P'||lv_cnt, TRIM(BOTH '"' FROM SUBSTR(lv_val1,lv_pos+1)));
      CASE 
      WHEN lv_data_type = 'NUMBER' THEN
           lv_col_predicate := 'TO_NUMBER(SYS_CONTEXT(''APP_CTX'',''$P'||lv_cnt||'''))';
      WHEN lv_data_type ='DATE' THEN
           lv_col_predicate := 'TO_DATE(SYS_CONTEXT(''APP_CTX'',''$P'||lv_cnt||'''),'''||lv_date_format||''')';
      WHEN lv_data_type LIKE '%TIMESTAMP%' THEN
           lv_col_predicate := 'TO_TIMESTAMP(SYS_CONTEXT(''APP_CTX'',''$P'||lv_cnt||'''),'''||lv_time_format||''')';
      ELSE
           lv_col_predicate := 'SYS_CONTEXT(''APP_CTX'',''$P'||lv_cnt||''')';
      END CASE;
      lv_where := REGEXP_REPLACE(lv_where,':'||lv_current_col,lv_col_predicate,1,1,'i');

  END LOOP;
  
  FOR lv_col IN (SELECT * FROM USER_TAB_COLUMNS WHERE TABLE_NAME = UPPER(p_tablename) ORDER BY COLUMN_ID)
  LOOP
      IF lv_col.COLUMN_ID=1 THEN
         lv_sql := 'SELECT ''{''';
      ELSE
         lv_sql := lv_sql||'||'',''';
      END IF;

      CASE 
      WHEN lv_col.DATA_TYPE ='DATE' THEN
           lv_sql := lv_sql||   '||''"'||lv_col.column_name||'":"''||TO_CHAR('||lv_col.column_name||','''||lv_date_format||''')||''"''';
      WHEN lv_col.DATA_TYPE LIKE '%TIMESTAMP%' THEN
           lv_sql := lv_sql||   '||''"'||lv_col.column_name||'":"''||TO_CHAR('||lv_col.column_name||','''||lv_time_format||''')||''"''';
      ELSE
           lv_sql := lv_sql||   '||''"'||lv_col.column_name||'":"''||'||lv_col.column_name||'||''"''';
      END CASE;

  END LOOP;
  lv_sql := lv_sql||'||''}'' AS json FROM '||p_tablename||' '||lv_where;
  
  IF p_page_size>0 AND p_page_idx>0 THEN
     lv_min := (p_page_idx-1)*p_page_size+1;
     lv_max := p_page_idx*p_page_size;
     lv_sql :='SELECT json FROM (SELECT json,ROWNUM rn FROM ('||lv_sql||') v WHERE ROWNUM<=:MAX_ROW) WHERE rn>= :MIN_ROW';
     OPEN p_output FOR lv_sql USING lv_max,lv_min;
  ELSE
     OPEN p_output FOR lv_sql;
  END IF;
  
  DBMS_OUTPUT.PUT_LINE(lv_sql);
  RETURN;
END read;

END get_data;
/

�����Ѿ���ȫ�����룬������¥������ײ��DAU_XXXX ��Щ�����Ĺ��ܣ�

�ͻ�����ô�����أ���¥���򵥶��ˣ���CALLһ�δ洢���̣������α꼴�ɡ�

������ģ����¥���Ĳ������ݣ�

CREATE TABLE trn_mmnt (trn_no VARCHAR2(20), stn_no NUMBER, stn_name VARCHAR2(20), trn_lvr VARCHAR2(10), arrive_time VARCHAR2(10), departure_time VARCHAR2(10), days NUMBER, run_minuts NUMBER, stop_minuts NUMBER);

insert into trn_mmnt values ('G7154', 1, '�Ϻ�����',  '����', ''     , '12:05', 0, 0,   0   );
insert into trn_mmnt values ('G7154', 2, '��ɽ��',    '����', '12:23', '12:24', 0, 18,  1   );
insert into trn_mmnt values ('G7154', 3, '����',      '����', '12:46', '12:48', 0, 41,  2   );
insert into trn_mmnt values ('G7154', 4, '����',      '����', '13:01', '13:02', 0, 56,  1   );
insert into trn_mmnt values ('G7154', 5, '����',      '����', '13:17', '13:18', 0, 72,  1   );
insert into trn_mmnt values ('G7154', 6, '��ͽ',      '����', '13:28', '13:29', 0, 83,  1   );
insert into trn_mmnt values ('G7154', 7, '��',      '����', '13:37', '13:38', 0, 92,  1   );
insert into trn_mmnt values ('G7154', 8, '�Ͼ�',      '����', '13:59', ''     , 0, 114, -1  );

��SQL PLUS�£�����һ���α꣺

VAR VRES REFCURSOR;

EXEC get_data.read(:vres,'trn_mmnt','where trn_no=:trn_no order by stn_no',3,1,'"trn_no":"G7154"');

--- �������
PRINT VRES;

JSON
------------------------------------------------------------------------------------------------------------------------------------------------------
{"TRN_NO":"G7154","STN_NO":"1","STN_NAME":"�Ϻ�����","TRN_LVR":"����","ARRIVE_TIME":"","DEPARTURE_TIME":"12:05","DAYS":"0","RUN_MINUTS":"0","STOP_MINUTS":"0"}
{"TRN_NO":"G7154","STN_NO":"2","STN_NAME":"��ɽ��","TRN_LVR":"����","ARRIVE_TIME":"12:23","DEPARTURE_TIME":"12:24","DAYS":"0","RUN_MINUTS":"18","STOP_MINUTS":"1"}
{"TRN_NO":"G7154","STN_NO":"3","STN_NAME":"����","TRN_LVR":"����","ARRIVE_TIME":"12:46","DEPARTURE_TIME":"12:48","DAYS":"0","RUN_MINUTS":"41","STOP_MINUTS":"2"}





