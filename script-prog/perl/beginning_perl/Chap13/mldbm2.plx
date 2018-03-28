#!/usr/bin/perl
#mldbm2.plx
use warnings;
use strict;
use SDBM_File;
use Storable;
use MLDBM qw(SDBM_File Storable);
use POSIX; #for O_CREAT and O_RDWR symbols
use strict;

my %mlhash;
my $mldb_file="mlanydbmdemo.dbm";

tie %mlhash, 'MLDBM', $mldb_file, O_CREAT|O_RDWR, 0644;
