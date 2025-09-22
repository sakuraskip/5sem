select pdb_name,status from CDB_PDBS; --task1
--
select * from v$instance; --task2
--
select comp_id,comp_name,version,status from dba_registry; --task3

select * from DBA_PDBS; --task5
--task9
create user C##SAV identified by 12345 container=ALL;
grant create session to C##SAV container=ALL;
--task 10
alter pluggable database SAV_PDB close immediate;
drop pluggable database SAV_PDB including datafiles;

drop user C##SAV cascade;