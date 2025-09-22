create tablespace pdb_data
datafile 'pdb-data.dbf'
size 50m
autoextend on 
next 5m
maxsize 100m
extent management local;

create temporary tablespace pdb_temp1
tempfile 'pdb-temp1.dbf'
size 20m
autoextend on
next 5m
maxsize 50m
extent management local uniform size 256k;

create profile LAB3PDB
limit
password_life_time 180
FAILED_LOGIN_ATTEMPTS 7
SESSIONS_PER_USER 3
PASSWORD_LOCK_TIME 1
password_reuse_time 10
connect_time 180
idle_time 30

create role LAB3ROLE;
grant create session to LAB3ROLE; 
grant create table to LAB3ROLE;
grant create view to LAB3ROLE;
grant create procedure to LAB3ROLE;
grant restricted session to LAB3ROLE;

create user LAB3USER identified by 12345
default tablespace pdb_data quota unlimited on pdb_data
temporary tablespace pdb_temp1
profile LAB3PDB
account unlock;
grant LAB3ROLE to LAB3USER;
--task8

select * from dba_tablespaces;

select * from dba_data_files;
select * from dba_temp_files;

select * from dba_roles;

select * from role_sys_privs;
select * from role_tab_privs;

select * from dba_profiles;

select * from dba_users;
select * from dba_role_privs;

