#!/usr/bin/perl
#mldbm1.plx
use warnings;
use strict;
use MLDBM;
use POSIX; #for O_CREAT and O_RDWR symbols
use strict;

my %mlhash;
my $mldb_file="mlanydbmdemo.dbm";

tie %mlhash, 'MLDBM', $mldb_file, O_CREAT|O_RDWR, 0644;
