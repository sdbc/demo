create or replace view vw_remant as 
select	to_char(on_date,'YYYY-MM-DD') as on_date,
	beg_station,
	run_train,
	car_no,
	seat_type as seat_type,
	end_station,
	purpose,
	count(*) as amoung
from seat
where flag=0 AND atart_date= ? AND beg_station=? AND run_train=? AND 
group by on_date,beg_station,run_train,car_no,seat_type,end_station,purpose 


/* newkid */
CREATE OR REPLACE VIEW vw_seat_remant 
AS   
SELECT on_date               
      ,beg_station           
      ,run_train             
      ,Carno                 
      ,seat_type             
      ,purpose               
      ,end_station           
      ,COUNT(*)  AS amoung   
  FROM seat
WHERE flag =0    --- �Ҽ����Ѿ��۳���Ʊ���flag��ı䣬flag=0��ʾû������ȥ�ġ����Լ����Ը�����Ҫ�޸�
GROUP BY on_date  
     ,beg_station 
     ,run_train   
     ,Carno       
     ,seat_type   
     ,purpose     
     ,end_station ;

