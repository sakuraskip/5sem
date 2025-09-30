create table lab2table(
    id number primary key,
    texttext varchar(50),
    num number)
    
create view lab2view as
select * from lab2table where id >2 --task 10
-------------------------------------------------
create table lab2task11(
    id number primary key,
    text varchar(100)) tablespace SAV_QDATA
    
insert into lab2task11 (id,text) values(1,'dsadsadsadas');
insert into lab2task11 (id,text) values(2,'das');
insert into lab2task11 (id,text) values(3,'dsas');
