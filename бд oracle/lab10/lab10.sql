--task1/

alter table TEACHER add ( birthday date, salary number);

--task2

DECLARE 
    start_date DATE;
    i NUMBER := 0;
BEGIN
    start_date := ADD_MONTHS(SYSDATE, -12*50);
    
    FOR teacher_rec IN (SELECT teacher FROM teacher ORDER BY teacher) LOOP
        i := i + 1;
        UPDATE teacher SET 
            birthday = start_date + 
                      (i * 400) +           
                      (MOD(i * 67, 300)),  
            salary = 40000 + (i * 5000)        
        WHERE teacher = teacher_rec.teacher;
    END LOOP;
    COMMIT;
END;
/
/
select substr(teacher_name,0,instr(teacher_name,' ')) ||
substr(teacher_name,instr(teacher_name,' ')+1,1) || '.' ||
substr(TEACHER_NAME, INSTR(TEACHER_NAME, ' ', 1, 2) + 1, 1) as Фио from teacher;
/
--task3
select * from teacher where to_char(birthday, 'D') = '2';
/
--task4
CREATE OR REPLACE VIEW task4 AS
SELECT TEACHER, TEACHER_NAME, BIRTHDAY, PULPIT
FROM TEACHER
WHERE EXTRACT(MONTH FROM BIRTHDAY) = EXTRACT(MONTH FROM ADD_MONTHS(SYSDATE, 1));

select * from task4;
drop view task4;

--task5
/
create or replace view task5 as
select to_char(birthday, 'Month') as month_name, count(*) as teacher_count
from teacher
group by to_char(birthday, 'MM'), to_char(birthday, 'Month');

select * from task5;
drop view task5;
/
--task6

declare
cursor task6 is 
select teacher_name, birthday from teacher where
mod(extract(year from sysdate) - extract(year from birthday),10) = 0;
c_teacher_name teacher.teacher_name%type;
c_birthday teacher.birthday%type;

begin
open task6;
loop
    fetch task6 into c_teacher_name, c_birthday;
    exit when task6%notfound;
    
    dbms_output.put_line(
    c_teacher_name || ' ' || c_birthday || ' '
    );
    end loop;
    close task6;
end;
/
--task7
DECLARE 
    CURSOR task7 IS
        SELECT 
            p.pulpit,
            f.faculty,
            FLOOR(AVG(t.salary)) as avg_salary,
            GROUPING(p.pulpit) as pulpit_group,
            GROUPING(f.faculty) as faculty_group
        FROM teacher t
        INNER JOIN pulpit p ON t.pulpit = p.pulpit
        INNER JOIN faculty f ON p.faculty = f.faculty
        GROUP BY rollup(f.faculty, p.pulpit)
        order by f.faculty;

    c_pulpit pulpit.pulpit%TYPE;
    c_pulpit_name pulpit.pulpit_name%TYPE;
    c_faculty faculty.faculty%TYPE;
    c_faculty_name faculty.faculty_name%TYPE;
    c_avg_salary NUMBER;
    c_pulpit_group NUMBER;
    c_faculty_group NUMBER;
BEGIN
    OPEN task7;
    LOOP
        FETCH task7 INTO c_pulpit, c_faculty, c_avg_salary, c_pulpit_group, c_faculty_group;
        EXIT WHEN task7%NOTFOUND;
        
        IF c_faculty_group = 1 THEN
            DBMS_OUTPUT.PUT_LINE('средняя зп: ' || c_avg_salary);
        ELSIF c_pulpit_group = 1 THEN
            DBMS_OUTPUT.PUT_LINE('средняя зп по факультету ' || trim(c_faculty) || ' ' || c_avg_salary);
        ELSE
            DBMS_OUTPUT.PUT_LINE('средняя зп по кафедре: ' || trim(c_pulpit) || ' ' || c_avg_salary);   
            
        END IF;
    END LOOP;
    CLOSE task7;
END;
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





