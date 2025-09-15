drop table SAV_t;

create table SAV_t(x number(3) primary key, s varchar(50));

insert into sav_t(x,s) values (1,'brr');
insert into sav_t(x,s) values (2,'arrr');
insert into sav_t(x,s) values(3,'abbb');
insert into sav_t(x,s) values (4,'arr1r');

commit;

update SAV_t set s='update1'
where s like '%a%';
commit;

select count(*) as amount from SAV_t where x > 1;
select * from SAV_t where s like '%r';
select min(x) from SAV_t;

delete from SAV_t where x=2;
commit;

create table SAV_t1(y number primary key, str varchar(50),
frkey number,
constraint frkey_const foreign key(frkey) references SAV_t(x)
);
insert into SAV_t1(y,str,frkey) values (7,'aaab',1);
-- insert into SAV_t1(y,str,frkey) values (8,'aaab',6);
insert into SAV_t1(y,str,frkey) values (9,'aaab',3);
insert into SAV_t1(y,str,frkey) values (13,'adaaab',3);

commit;

select SAV_t.x,SAV_t.s,SAV_t1.y,SAV_t1.frkey
from SAV_t inner join SAV_t1 on sav_t.x = sav_t1.frkey;

select SAV_t.x,SAV_t.s,SAV_t1.y,SAV_t1.frkey
from SAV_t left join SAV_t1 on sav_t.x = sav_t1.frkey;

select SAV_t.x,SAV_t.s,SAV_t1.y,SAV_t1.frkey
from SAV_t right join SAV_t1 on sav_t.x = sav_t1.frkey;

drop table SAV_t;
drop table SAV_t1;
