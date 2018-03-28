#!/usr/bin/perl
#opendbm2.plx
use warnings;
use strict;
use POSIX;
use SDBM_File;           # or GDBM_File / NDBM_File / AnyDBM_File...

my %dbm;
my $db_file="demo.dbm";

tie %dbm, 'SDBM_File', $db_file, O_RDWR, 0 or die "Error opening $db_file: $!\n";
