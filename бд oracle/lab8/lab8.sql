begin
    null;
end;
--task2
/
begin
    DBMS_OUTPUT.PUT_LINE('hello word');
    end;
/
--task3

declare result number;
begin
    result := 1/0;
    exception
    when ZERO_DIVIDE then
    DBMS_OUTPUT.PUT_LINE('ERROR: zero divide');
    DBMS_OUTPUT.PUT_LINE('sqlerr' || SQLERRM);
    DBMS_OUTPUT.PUT_LINE('sqlcode' || sqlcode);
end;
/
--task4
declare  
begin
    DBMS_OUTPUT.PUT_LINE('внешний блоК');
    
    declare  
        V_RESULT number;
    begin
        DBMS_OUTPUT.PUT_LINE('внутренний БЛоК');
        V_RESULT := 1/0;  
        
    exception
        when ZERO_DIVIDE then
            DBMS_OUTPUT.PUT_LINE('internal ERROR : ZERODIVIDE' || sqlerrm);
    end;
        
exception
    when others then
        DBMS_OUTPUT.PUT_LINE('external eror: ' || sqlcode);
end;
/
--task5
select * from v$parameter where name like '%warning%' or name like '%plsql%';
--task6
select keyword from v_$reserved_words where length = 1;
--task7
select keyword from v_$reserved_words where length > 1 order by keyword;
--task8
select * from v$parameter where name like '%plsql%';
show parameter plsql;
--task9-17

declare 
    int1 number := 100;
    int2 number := 200;
    int_res number;

    fixed1 number(10,2) := 123.45;
    fixed2 number(10,3) := 1234.567;

    round1 number(5,-2) := 1234;
    round2 number(7,-4) := 5678;

    binaryFloat binary_float := 7.1231f;
    binaryDouble binary_double := 4.12313123131d;

    exponent1 number := 1.2E+5;
    exponent2 number := 2.4E-4;  

    bool_1 boolean := true;
    bool_2 boolean := false;

begin
    
    dbms_output.put_line('int1: ' || int1);
    dbms_output.put_line('int2: ' || int2);
    dbms_output.put_line('int_res: ' || nvl(to_char(int_res), 'NULL'));
    
    dbms_output.put_line('fixed1: ' || fixed1);
    dbms_output.put_line('fixed2: ' || fixed2);
    
    dbms_output.put_line('round1: ' || round1);
    dbms_output.put_line('round2: ' || round2);
    
    dbms_output.put_line('binaryFloat: ' || binaryfloat);
    dbms_output.put_line('binaryDouble: ' || binarydouble);
    
    dbms_output.put_line('exponent1: ' || exponent1);
    dbms_output.put_line('exponent2: ' || exponent2);
    
    dbms_output.put_line('bool_1: ' || CASE WHEN bool_1 THEN 'TRUE' WHEN NOT bool_1 THEN 'FALSE' END);
    dbms_output.put_line('bool_2: ' || CASE WHEN bool_2 THEN 'TRUE' WHEN NOT bool_2 THEN 'FALSE' END);
    
    dbms_output.put_line('');
    dbms_output.put_line('sum: ' || (int1 + int2));
    dbms_output.put_line('sub: ' || (int1 - int2));
    dbms_output.put_line('multi: ' || (int1 * int2));
    dbms_output.put_line('division: ' || (int1 / int2));
    dbms_output.put_line('mod: ' || MOD(int1, int2));
    
    dbms_output.put_line('');
    dbms_output.put_line('fixed1 + fixed2: ' || (fixed1 + fixed2));
    dbms_output.put_line('fixed1 * fixed2: ' || (fixed1 * fixed2));
    
    dbms_output.put_line('');
    dbms_output.put_line('round1 (1234 → ' || round1 || ')');
    dbms_output.put_line('round2 (5678 → ' || round2 || ')');
    
    dbms_output.put_line('');
    dbms_output.put_line('exponent1 (1.2E+5): ' || exponent1 || ' = 120000');
    dbms_output.put_line('exponent2 (2.4E-4): ' || exponent2 || ' = 0.00024');

end;
/
--task18
DECLARE
  v_const_varchar CONSTANT VARCHAR2(10) := 'lab';
  v_const_char   CONSTANT CHAR(5) := 'eight';
  v_const_number CONSTANT NUMBER := 100;
BEGIN
  DBMS_OUTPUT.PUT_LINE(v_const_varchar || ' ' || v_const_char);
  DBMS_OUTPUT.PUT_LINE('Number: ' || v_const_number);
  DBMS_OUTPUT.PUT_LINE('Number * 2: ' || v_const_number * 2);
END;
/
--task19
declare task19 v$parameter.name%type;
begin

task19:='nametype name';
dbms_output.put_line('task19: ' || task19);
end;
/
--task20
declare
  TASK20 V$PARAMETER%ROWTYPE;
begin
  select * into TASK20 from V$PARAMETER where num =141;
  DBMS_OUTPUT.PUT_LINE('task20: ' || TASK20.name || ' ' || TASK20.value);
end;
/
--task21
DECLARE
  v_number NUMBER := 10;
BEGIN
  IF v_number > 0 THEN
    DBMS_OUTPUT.PUT_LINE('> 0');
    else
    dbms_output.put_line('< 0');
  END IF;


  IF v_number < 0 THEN
    DBMS_OUTPUT.PUT_LINE('< 0');
  ELSIF v_number = 0 THEN
    DBMS_OUTPUT.PUT_LINE('zero');
  ELSE
    DBMS_OUTPUT.PUT_LINE('plus ');
  END IF;
END;
/
--task23
DECLARE
  caseNumber number := 4;
  v_result VARCHAR2(50);
BEGIN
  v_result := 
    CASE caseNumber
      WHEN 4 THEN 'four'
      WHEN 5 THEN 'five'
      WHEN 6 THEN 'six'
      ELSE 'not four not five not six'
    END;
  DBMS_OUTPUT.PUT_LINE('result: ' || v_result);

  v_result := 
    CASE
      WHEN caseNumber = 5 THEN 'five'
      WHEN caseNumber IN (4,6) THEN 'four or six'
      ELSE 'not'
    END;
  DBMS_OUTPUT.PUT_LINE('result 2: ' || v_result);
END;
/
--task24
DECLARE
  loopnumber NUMBER := 1;
BEGIN
  LOOP
    DBMS_OUTPUT.PUT_LINE('loop: ' || loopnumber);
    loopnumber := loopnumber + 1;
    EXIT WHEN loopnumber >  10;
  END LOOP;
END;
/
--task25
DECLARE
  loopnumber NUMBER := 1;
BEGIN
  WHILE loopnumber <= 5 LOOP
    DBMS_OUTPUT.PUT_LINE('while: ' || loopnumber);
    loopnumber := loopnumber + 1;
  END LOOP;
END;
/
--task26
BEGIN
  FOR i IN 1..5 LOOP
    DBMS_OUTPUT.PUT_LINE('for loop: ' || i);
  END LOOP;
END;
/





