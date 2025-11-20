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
declare  -- Внешний блок
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
