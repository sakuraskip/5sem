--task2

create sequence s1 
start with 1000
increment by 10
nominvalue
nomaxvalue
nocycle
nocache;

select s1.nextval from dual;
select s1.nextval from dual;
select s1.currval from dual;


--task3-4
create  sequence s2
start with 10
increment by 10
maxvalue 100
nocycle;

select s2.nextval from dual connect by level <=10;
select s2.nextval from dual;

--task5
create sequence s3
start with 10
increment by -10
minvalue -100
maxvalue 10
nocycle
order;

select s3.nextval from dual connect by level <=12;
select s3.nextval from dual;

--task6
create sequence s4
start with 10
increment by 1
minvalue 10
maxvalue 15
cycle
cache 5
noorder;

select s4.nextval from dual connect by level <= 15;


--task7
select * from all_sequences where sequence_owner like 'SAV';

--task8
create table A(
N1 number(20),
N2 number(20),
N3 number(20),
N4 number(20)) storage(buffer_pool keep)
cache;

--from sys
alter user SAV quota unlimited on users;

begin
for i in 1..7 loop
insert into A(N1,N2,N3,N4)
values (s1.nextval, s2.nextval,s3.nextval,s4.nextval);
end loop
commit;
end;
/
select * from A;
--task9-12
create cluster abc(
x number(10),
v varchar(12)) hashkeys 200;

create table A1(
xa number(10),
va varchar(12),
text varchar(50)) cluster abc (xa,va);

create table b(
xb number(10),
vb varchar(12),
text varchar(50)) cluster abc (xb,vb);

create table c(
xc number(10),
vc varchar(12),
text varchar(50)) cluster abc (xc,vc);

--task13
select buffer_pool from user_tables where table_name in ('A1','B','C');
select * from user_clusters where cluster_name like 'ABC';

--task14
create synonym t14 for SAV.C;

select * from t14;
--task15
create public synonym t15p for SAV.B;

select * from t15p;

--task16
create table t16A(
id number primary key,
text varchar(50));

create table t16B(
id number primary key,
t16k number,
text1 varchar(50),
constraint A_key foreign key (t16k) references t16A(id));

insert into t16A values(1,'asdd');
insert into t16A values(2,'bbs');
insert into t16A values(3,'zxczc');

insert into t16B values(1,2,'text3');
insert into t16B values(2,3,'cvcvxv');

create view v16 as 
select t16A.id as a_id, t16A.text, t16B.id, t16B.text1
from t16A inner join t16b on t16A.id = t16B.t16k;

select * from v16;


--task17
create materialized view log on SAV.t16A
with primary key, sequence including new values;

create materialized view log on SAV.t16B
with primary key, sequence including new values;

create materialized view mv 
refresh force start with sysdate next sysdate + (15/(24*60*60))
as select t16A.id as a_id, t16A.text, t16B.id as b_id, t16B.text1
from t16A inner join t16b on t16A.id = t16B.t16k;

insert into t16B values(3,3,'newtext');-- insert, then select, then exec, then select

select * from mv;
exec dbms_mview.refresh('MV','COMPLETE');
select * from mv;
--------------------------
drop materialized view mv;
drop view v16;
drop table t16B purge;
drop table t16A purge;
drop synonym t14;
drop public synonym t15p;

drop table A1 purge;
drop table B purge;
drop table C purge;
drop cluster ABC;
drop table A purge;
drop sequence s4;
drop sequence s3;
drop sequence s2;
drop sequence s1;








