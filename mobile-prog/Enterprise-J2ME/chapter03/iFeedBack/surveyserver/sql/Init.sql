drop table iFeedBackQs;
drop table iFeedBackAs;
 
create table iFeedBackQs
(
   qid      integer,
   question varchar(1024),
   choices  varchar(2048),
   visible  boolean
);

create table iFeedBackAs
(
   userid    varchar(128),
   qid       integer,
   answer    varchar(1024),
   comments  varchar(2048),
   ts        timestamp
);

insert into iFeedBackQs values (1, 
   'How do you evaluate the class progress today?', 
   'too slow, I already knew:too fast, I do not understand:about right', 
                           true); 
