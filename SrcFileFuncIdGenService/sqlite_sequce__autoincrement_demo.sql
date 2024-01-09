-- sqlite_sequce__autoincrement_demo.sql
create table tbl_test(uid INTEGER , age integer,
PRIMARY KEY("uid" AUTOINCREMENT)
);

insert into tbl_test(uid,age) values(257,20);
insert into tbl_test(age) values(55);
delete from tbl_test;

select * from sqlite_sequence where name='tbl_test';
-- sqlite_sequence : name==tbl_test, seq==258
