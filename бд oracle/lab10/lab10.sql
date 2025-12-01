--task1/

alter table teacher add ( birthday date, salary number);
select * from teacher;
--task2
/
declare 
    start_date date;
    i number := 0;
begin
    start_date := add_months(sysdate, -12*50);
    
    for teacher_rec in (select teacher from teacher order by teacher) loop
        i := i + 1;
        update teacher set 
            birthday = start_date + 
                      (i * 400) +           
                      (mod(i * 67, 300)),  
            salary = 40000 + (i * 5000)        
        where teacher = teacher_rec.teacher;
    end loop;
    commit;
end;
/

select substr(teacher_name,0,instr(teacher_name,' ')) ||
substr(teacher_name,instr(teacher_name,' ')+1,1) || '.' ||
substr(teacher_name, instr(teacher_name, ' ', 1, 2) + 1, 1) as фио from teacher;
/
--task3
select * from teacher where to_char(birthday, 'D') = '1'; -- monday
/
--task4
create or replace view task4 as
select trim(teacher) as teacher, teacher_name, birthday, pulpit
from teacher
where extract(month from birthday) = extract(month from add_months(sysdate, 1));

select * from task4;
drop view task4;
--task5
/
create or replace view task5 as
select to_char(birthday, 'Month') as month_name, count(*) as teacher_count
from teacher
group by to_char(birthday, 'Month');

select * from task5;
drop view task5;
/
--task6

declare
cursor task6 is 
select teacher_name, birthday from teacher where
mod((extract(year from sysdate)+1) - extract(year from birthday),10) = 0;
c_teacher_name teacher.teacher_name%type;
c_birthday teacher.birthday%type;

begin
open task6;
loop
    fetch task6 into c_teacher_name, c_birthday;
    exit when task6%notfound;
    
    dbms_output.put_line(
    trim(c_teacher_name) || ' ' || c_birthday || ' '
    );
    end loop;
    close task6;
end;
/
--task7
declare 
    cursor task7 is
        select 
            p.pulpit,
            f.faculty,
            floor(avg(t.salary)) as avg_salary,
            grouping(p.pulpit) as pulpit_group,
            grouping(f.faculty) as faculty_group
        from teacher t
        inner join pulpit p on t.pulpit = p.pulpit
        inner join faculty f on p.faculty = f.faculty
        group by rollup(f.faculty, p.pulpit)
        order by f.faculty;

    c_pulpit pulpit.pulpit%type;
    c_pulpit_name pulpit.pulpit_name%type;
    c_faculty faculty.faculty%type;
    c_faculty_name faculty.faculty_name%type;
    c_avg_salary number;
    c_pulpit_group number;
    c_faculty_group number;
begin
    open task7;
    loop
        fetch task7 into c_pulpit, c_faculty, c_avg_salary, c_pulpit_group, c_faculty_group;
        exit when task7%notfound;
        
        if c_faculty_group = 1 then
            dbms_output.put_line('средняя зп: ' || c_avg_salary);
        elsif c_pulpit_group = 1 then
            dbms_output.put_line('средняя зп по факультету ' || trim(c_faculty) || ' ' || c_avg_salary);
        else
            dbms_output.put_line('средняя зп по кафедре: ' || trim(c_pulpit) || ' ' || c_avg_salary);   
            
        end if;
    end loop;
    close task7;
end;
/

--task8

declare
type task8 is record(
    fullname teacher.teacher_name%type,
    birthday teacher.birthday%type,
    salary teacher.salary%type,
    coolNumber number);
    
    type task8Out is record(
    innerfield task8,
    coolChar varchar2(50));
    
    var1 task8;
    var2 task8;
    var3 task8Out;
    begin
    
    var1.fullname:= 'fullname var1';
    var1.birthday := sysdate;
    var1.salary :=1356;
    var1.coolNumber :=6.2;
    
    dbms_output.put_line('var1: ' || var1.fullname || ' ' || var1.salary || ' ' || var1.coolNumber);
    var2 := var1;
    dbms_output.put_line('var2: ' || var2.fullname || ' ' || var2.salary || ' ' || var2.coolNumber);
    
    var3.innerfield := var2;
    var3.coolChar := '13:56';
    dbms_output.put_line('var3 salary: ' || var3.innerfield.salary );
    end;
    /





