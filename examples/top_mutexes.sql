with top_mutexes as (
   select--+ leading(t1 s1 v1 v2 t2 s2)  use_hash(s1) use_nl(v1) use_hash(s2) materialize
      t1.hsecs
     ,s1.*
     ,s2.sleeps           as end_sleeps
     ,s2.wait_time        as end_wait_time
     ,s2.sleeps-s1.sleeps as delta_sleeps
     ,t2.HSECS - t1.HSECS as delta_hsecs
     --,s2.* 
   from v$timer t1
       ,v$mutex_sleep s1
       ,(select/*+ no_merge */ sum(level) a from dual connect by level<=1e6) v1
       ,v$timer t2
       ,v$mutex_sleep s2
   where s1.MUTEX_TYPE=s2.MUTEX_TYPE
   and s1.location=s2.location
)
select *
from top_mutexes
order by delta_sleeps desc;
