select * from dba_data_files;
select * from dba_temp_files;

--task2
create tablespace SAV_QDATA 
datafile 'qdata.dbf'
size 10m
offline;



alter tablespace SAV_QDATA online;

create user C##SAV identified by 12345;
alter user C##SAV quota 2m on SAV_QDATA;

create role C##LAB3ROLE;
grant create session to C##LAB3ROLE; 
grant create table to C##LAB3ROLE;
grant create view to C##LAB3ROLE;
grant create procedure to C##LAB3ROLE;
grant restricted session to C##LAB3ROLE;

grant C##LAB3ROLE to C##SAV;
--connect as SAV

create table SAV_T1(
id number primary key,
text varchar(50)) TABLESPACE SAV_QDATA;

insert into SAV_T1(id,text) values (1,'adasda');
insert into SAV_T1(id,text) values (2,'sdgbg');
insert into SAV_T1(id,text) values (3,'erewrw');
select * from SAV_T1;
--task3 from sys
select * from dba_segments where tablespace_name = 'SAV_QDATA';
select * from dba_segments where tablespace_name = 'SAV_QDATA'
and segment_name = 'SAV_T1';

select * from dba_segments where tablespace_name = 'SAV_QDATA'
and not segment_name = 'SAV_T1';

--task4
drop table SAV_T1;-- connect as sav
--connect as sys
select * from dba_segments where tablespace_name = 'SAV_QDATA';
select * from dba_segments where tablespace_name = 'SAV_QDATA'
and segment_name = 'SAV_T1';

select * from USER_RECYCLEBIN;--connect as sav
--task5
flashback table SAV_T1 to before DROP;
--task6
begin
    for i in 5 .. 10005 loop
        insert into SAV_T1(id,text) values (i, 'num: ' || i);
    end loop;
    commit;
end;
select * from SAV_T1;
--task7, connect as system
select count(*) as extentCount, sum(blocks) as blocksCount,
sum(bytes) as bytesCount from dba_segments where segment_name = 'SAV_T1';

select * from dba_extents;
--task8
drop tablespace SAV_QDATA including contents and datafiles;
--task9
select * from v$log;
select * from v$log where status='CURRENT';
--task10
select * from v$logfile;
--task11
alter system switch logfile; --+task12
select TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS') from dual; --09:43:21
--task12
select group# from v$log;
alter database add logfile group 5
(
    'redo05_1.log',
    'redo05_2.log',
    'redo05_3.log'
) size 30m;


select * from v$logfile where group#=5;
select * from v$log order by sequence# desc;--scn

--task13
alter database drop logfile group 5; --execute shell script, after closing db execute again
--task14+15
archive log list;
select * from v$archived_log;
--task16
shutdown immediate;--sqlplus
startup mount;
alter database archivelog;
alter database open;

archive log list;
--task17
alter system archive log current;
select * from v$archived_log order by sequence# desc;

select * from v$log order by sequence# desc;--redo journal
select * from v$archived_log order by sequence# desc;--archive journal
--task18
shutdown immediate;--sqlplus
startup mount;
alter database noarchivelog;
alter database open;

archive log list;
--task19
select * from v$controlfile;
--task20
select * from v$parameter; --tablespaces, redo logs, scn, checkpoint info, datafiles, journals
--task21
select * from v$parameter where name = 'spfile';
--task22
create pfile='sav_pfile.ora' from spfile;
--task23
--product\12.2.0\dbhome_1\database
--task24

SELECT value FROM v$parameter WHERE name = 'diagnostic_dest';
show parameter background_dump_dest; -- product\12.2.0\dbhome_1\rdbms\trace
--task25 C:\sfggfgdsg\diag\rdbms\orcl\orcl\trace
--task26 C:\SFGGFGDSG\PRODUCT\12.2.0\DBHOME_1\RDBMS\.0001 file

drop role C##LAB3ROLE;
drop user C##SAV cascade;




select * from dba_recyclebin














