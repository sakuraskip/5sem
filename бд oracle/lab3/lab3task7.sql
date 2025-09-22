create table SAV_table(
x number primary key,
tty varchar(50));

insert into SAV_table(x,tty) values (3,'aaa');
insert into SAV_table(x,tty) values (4,'bbb');

select * from SAV_table;
--
