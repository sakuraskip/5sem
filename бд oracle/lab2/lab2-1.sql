create tablespace TS_SAV
datafile 'TS_SAV2.dbf'
size 7M
autoextend on
next 5m
maxsize 20m
extent management local;
--drop tablespace TS_SAV including contents and datafiles;
create temporary tablespace TS_SAV_temp
tempfile 'TS_SAV2_temp.dbf'
size 5m
autoextend on
next 3m
maxsize 30m
extent management local uniform size 256k;
--drop tablespace TS_SAV_temp including contents and datafiles;
--
select tablespace_name from dba_tablespaces;
select * from dba_data_files;
select  * from dba_temp_files;

create role C##RL_SAVCORE;
grant create session to C##RL_SAVCORE; 
grant create table to C##RL_SAVCORE;
grant create view to C##RL_SAVCORE;
grant create procedure to C##RL_SAVCORE;
--drop role C##RL_SAVCORE;

select * from dba_roles where ROLE like 'C##RL_SAVCORE';
select * from dba_sys_privs where GRANTEE like 'C##RL_SAVCORE';

--
create profile C##PF_SAVCORE
limit
password_life_time 180
FAILED_LOGIN_ATTEMPTS 7
SESSIONS_PER_USER 3
PASSWORD_LOCK_TIME 1
password_reuse_time 10
connect_time 180
idle_time 30;

--drop profile C##PF_SAVCORE cascade;

select * from dba_profiles;
select * from dba_profiles where profile ='C##PF_SAVCORE';
select * from dba_profiles where profile ='DEFAULT';

create user C##SAVCORE identified by 12345
default tablespace TS_SAV quota unlimited on TS_SAV
temporary tablespace TS_SAV_temp
profile C##PF_SAVCORE
account unlock
password expire;
--drop user C##SAVCORE cascade;


grant C##RL_SAVCORE to C##SAVCORE;

create tablespace SAV_QDATA
datafile 'qdata1.dbf'
size 10m
autoextend on
next 5m
maxsize 20m
offline
extent management local;
alter tablespace SAV_QDATA online;

--drop tablespace SAV_QDATA including contents and datafiles;


alter user C##SAVCORE quota 2m on SAV_QDATA;

