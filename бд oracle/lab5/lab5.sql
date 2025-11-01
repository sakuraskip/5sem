--task1
select (sum(VALUE)/1024/1024) as mb from v$sga;
--task2
select name, round(BYTES/1024/1024) as mb from v$sgainfo; --fixed sga, redo buffer,
--buffer cache, shared pool
--task3
select component, granule_size/1024/1024 as granule_size from v$sga_dynamic_components;
--task4
select name, bytes/1024/1024 as mb from v$sgastat where name = 'free memory';
--task5
select name, value/1024/1024 as mb from v$parameter where name like '%sga_max_size%'
or name like '%sga_target%';
--task6
select * from v$sga_dynamic_components where component like '%KEEP%' or
component like '%RECYCLE%' or component like '%DEFAULT%';
--auto memory management, so no recycle and keep pools
select * from v$parameter where name like '%sga_target%' or name like '%sga_max_size%';

--task7
create table task7 (
id number primary key,
text varchar(50)
) storage (BUFFER_POOL keep);
insert into task7(id,text) values (1,'adasda');
insert into task7(id,text) values (2,'sdgbg');
insert into task7(id,text) values (3,'erewrw');

select * from dba_segments where segment_name = 'TASK7';

drop table task7 purge;
--task8
create table task8 (
id number primary key,
text varchar(50)
) storage (BUFFER_POOL default);
insert into task8(id,text) values (1,'adasda');
insert into task8(id,text) values (2,'sdgbg');
insert into task8(id,text) values (3,'erewrw');

select * from dba_segments where segment_name = 'TASK8';

--task9
select name, value/1024/1024 as mb from v$parameter where name like '%log_buffer%';

--task10
select pool, name, bytes/1024/1024 as mb from v$sgastat where pool like '%large pool%';

--task11
select server, username, type from v$session where type like '%USER%';

--task12
select * from v$bgprocess;
--task13
--background = null -> system
select * from v$process;

--task14
select count(*) as proc_count from v$bgprocess where name like '%DBW%';

--task15
select * from dba_services;

--task16
select * from v$dispatcher;
select * from v$parameter where name like '%shared%' or name like '%dispatchers%';

--task17 services.msc -> oracleListener

--task18
select * from v$parameter where name = 'local_listener';

--task19 lsnrctl status, start, stop ,reload

--task 20 lsnrctl services











