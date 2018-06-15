CREATE GLOBAL TEMPORARY TABLE seat_parameter (    -- ������
          dflg           NUMBER(1)    -- ����JOBDAY�ļ���
         ,start_date     DATE         -- today
         ,end_date       DATE         -- ����ǿգ���������ɶ��������
         ,train_no       VARCHAR2(12) -- ���Ϊ0��NULL�����
          ) ON COMMIT DELETE ROWS
/

-- ���������Ӳ�ѯ�����������ͼ��
CREATE OR REPLACE VIEW vw_train
AS  ------ �޺ź���ͨ���õ���һЩ�任����͹����Ӳ�ѯ(���κ�ͣ��վ��ѡȡ)
SELECT t.train_no
      ,t.gride  
      ,t.station_num
      ,t.jobday
      ,t.item
      ,cz.station_code                                           AS beg_station
      ,t.jobday+(t.start_time+cz.running_time+cz.stop_time)/1440 AS on_date
      ,cz.run_train_no                                           AS run_train 
      ,cz.over
      ,cz.sequence
  FROM (SELECT t2.*
              ,SUBSTR(cycle,MOD(jobday-beg_date, LENGTH(cycle))+1,1)   AS item -- ԭ����д�˸�LENGTH
         FROM (SELECT train.* 
                     ,(CASE WHEN NVL(p.dflg,0)=0 THEN p.today + (CASE WHEN train.days>0 THEN train.days ELSE 10 END)
                            ELSE today
                       END) AS jobday     --- ���dflgΪ�����train����ȡ��
               FROM train
                   ,(SELECT dflg   -- ����end_date,start_date���һ��today�ļ��ϡ������Ȼ�end_dateΪ����ֻ��һ��
                           ,start_date + ROWNUM -1 AS today
                           ,train_no
                       FROM seat_parameter
                     CONNECT BY ROWNUM <= NVL(end_date,start_date) - start_date +1
                     ) p     
               WHERE train.flag=0 AND train.end_date >= p.today 
                     AND (train.train_no=p.train_no OR NVL(p.train_no,'0')='0')
              ) t2
        ) t
      ,stop_station cz
WHERE t.jobday BETWEEN t.beg_date AND t.end_date
       AND t.item<>'0'
       AND cz.train_no=t.train_no 
       AND t.jobday BETWEEN cz.beg_date AND cz.end_date
       AND cz.flag=0
/


CREATE OR REPLACE VIEW vw_seat
AS    --- ����������ͼvw_train�͸����߼�����ϯλ
WITH tr AS (SELECT * FROM vw_train)
    ,vw_seat_no AS (SELECT ROWNUM as seat_no FROM DUAL CONNECT BY ROWNUM<=200)
SELECT -- ��һ���֣������޺�ϯλ
       tr.train_no          AS train_no         
      ,tr.gride             AS gride  
      ,tr.station_num-1     AS end_station      
      ,tr.jobday            AS start_date                                
      ,0                    AS carno                                     
      ,0                    AS purpose                                   
      ,0                    AS seat_type                                 
      ,0                    AS shortest_station                          
      ,0                    AS flag                                      
      ,tr.beg_station       AS beg_station      
      ,tr.on_date           AS on_date      
      ,tr.run_train         AS run_train    
      ,0                    AS pro          
      ,vw_seat_no.seat_no   AS seat_no      
FROM  tr
     ,vw_seat_no
WHERE --tr.station_num-1 > tr.sequence        
       tr.over >= vw_seat_no.seat_no     
UNION ALL -- �ڶ����֣�������ͨϯλ
SELECT tr.train_no          AS train_no              
      ,ps.gride             AS gride                 
      ,ps.end_station       AS end_station           
      ,tr.jobday            AS start_date            
      ,ps.carno             AS carno                       
      ,ps.purpose           AS purpose               
      ,(CASE WHEN ps.seat_type = 20 THEN 20+vw_seat_type_adj.rn         -- Ӳ����һϯ3�ţ������ϡ�seat_type:20=Ӳ�ԣ�21Ӳ���£�22Ӳ���У�23Ӳ����
             WHEN ps.seat_type = 30 THEN 31+MOD(vw_seat_no.seat_no+1,2) -- ������һϯһ�ŵ���˫��: 30���ԣ�31�����£�32�����ϡ�
             ELSE ps.seat_type
        END)                AS seat_type             
      ,ps.shortest_station  AS shortest_station      
      ,0                    AS flag                                     
      ,tr.beg_station       AS beg_station           
      ,tr.on_date           AS on_date               
      ,tr.run_train         AS run_train             
      ,0                    AS pro                   
      ,vw_seat_no.seat_no   AS seat_no               
  FROM tr
      ,pre_seat ps
      ,pre_seat_index psi
      ,vw_seat_no
      ,(SELECT ROWNUM AS rn FROM DUAL CONNECT BY ROWNUM<=3) vw_seat_type_adj
WHERE psi.train_no=tr.train_no AND psi.item=tr.item AND tr.jobday BETWEEN psi.beg_date AND psi.end_date AND psi.flag=0
       AND ps.pre_id = tr.train_no||':'||tr.item||':'||TO_CHAR(psi.beg_date,'YYYYMMDD') 
       AND tr.sequence = ps.beg_station   -- ���޺Ų�ͬ
       AND vw_seat_no.seat_no BETWEEN ps.beg_seatno AND ps.end_seatno
       AND (ps.seat_type=20       --- Ӳ�ԣ���������
            OR ps.seat_type <>20 AND vw_seat_type_adj.rn = 1   -- ��Ӳ�ԣ�ֻ����һ��
           )
/
CREATE OR REPLACE VIEW vw_new_seat
AS
WITH v AS (SELECT v.*, ROWNUM rn FROM vw_seat v)
SELECT v.* 
  FROM v LEFT JOIN seat s
       ON (s.start_date  = v.start_date  
           AND s.beg_station = v.beg_station 
           AND s.Train_no    = v.Train_no    
           AND s.Carno       = v.Carno       
           AND s.seat_no     = v.seat_no     
           AND s.seat_type   = v.seat_type   
           )
WHERE s.start_date IS NULL
/

--- �����洢��������ϯλ��
CREATE OR REPLACE PROCEDURE make_seats (
          p_train_no     IN VARCHAR2 DEFAULT NULL 
         ,p_dflg         IN NUMBER    DEFAULT 0    ---- ���1����TRAINS�����ƫ����       
         ,p_offset_days  IN NUMBER    DEFAULT 0    ---- ȱʡ������ƫ��Ϊ0, +1Ϊ�ڶ���
         ,p_created_days IN NUMBER    DEFAULT 1    ---- ȱʡһ��
         )
AS
   lv_timestamp DATE := SYSDATE;
BEGIN
   INSERT INTO seat_parameter (    -- ������
          dflg            -- ����JOBDAY�ļ���
         ,start_date      -- today
         ,end_date        -- ����ǿգ���������ɶ��������
         ,train_no        -- ���Ϊ0��NULL�����
          )
   VALUES(p_dflg        
         ,TRUNC(SYSDATE)+p_offset_days  
         ,TRUNC(SYSDATE)+p_offset_days + p_created_days -1
         ,p_train_no    
         );


   INSERT INTO seat (
          train_no               
         ,gride            -- grade?      
         ,end_station            
         ,start_date             
         ,carno                  
         ,purpose                
         ,seat_type              
         ,shortest_station       
         ,flag                   
         ,beg_station            
         ,on_date                
         ,run_train              
         ,pro                    
         ,seat_no                
         ,used_time              
         ,used_dev               
         ,used_uid               
         )
   SELECT train_no               
         ,gride            
         ,end_station            
         ,start_date             
         ,carno                  
         ,purpose                
         ,seat_type              
         ,shortest_station       
         ,flag                   
         ,beg_station            
         ,on_date                
         ,run_train              
         ,pro                    
         ,seat_no                
         ,lv_timestamp AS used_time              
         ,'xwfb' AS used_dev               
         ,'xwfb' AS used_uid               
     FROM vw_new_seat;

   MERGE INTO pre_seat_index psi
   USING (SELECT DISTINCT psi.ROWID rid
            FROM pre_seat_index psi
                ,vw_train tr
           WHERE psi.train_no=tr.train_no AND psi.item=tr.item AND tr.jobday BETWEEN psi.beg_date AND psi.end_date AND psi.flag=0
         ) n
   ON (n.rid = psi.ROWID)
   WHEN MATCHED THEN UPDATE
        SET psi.distribute_time = lv_timestamp;

   COMMIT;

END make_seats;
/
SHOW ERROR PROCEDURE make_seats;
