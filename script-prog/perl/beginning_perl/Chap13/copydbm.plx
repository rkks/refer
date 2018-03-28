#!/usr/bin/perl
#copydbm.plx
use warnings;
use strict;
use POSIX;
use NDBM_File;
use GDBM_File;

my (%ndbm_db,%gdbm_db);
my $ndbm_file='/tmp/my_old_ndbm_database';
my $gdbm_file='/tmp/my_new_gdbm_database';

tie %ndbm_db, 'NDBM_File',$ndbm_file, O_RDONLY, 0;
tie %gdbm_db, 'GDBM_File',$gdbm_file, O_CREAT|O_WRONLY, 0644;

%gdbm_db=%ndbm_db;

untie %ndbm_db;
untie %gdbm_db;
