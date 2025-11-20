--task2 sqlplus system/{passwd}@//host:port/{pdbname}

--task3
select tablespace_name from dba_tablespaces;
select file_name from dba_data_files;
select role from dba_roles;
select username from dba_users;
--task4 regedit
--task5 sqlplus C##SAV/12345@SAV_SID

--task7 DO NOT FORGET TO COMMIT 
create table lab6(
id number primary key,
text varchar(50));
insert into lab6(id,text) values(1,'gsdfffdgfdgd');

select * from lab6;
drop table lab6 purge;

--task8 help timing -> set timing on -> select
--task9 describe {tablename}
--task10 select segment_name from user_segments;

--task11 connect as sys in sqlplus too
create view task11 as 
select count(*) as seg_count,
sum(extents) as extents_count,
sum(blocks) as blocks_count,
sum(bytes)/1024 as kb_count from dba_segments;

drop view task11;

