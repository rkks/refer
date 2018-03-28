#!/usr/bin/perl
#mldbm3.plx
use warnings;
use strict;
BEGIN {
    @AnyDBM_File::ISA = qw(GDBM_File DB_File NDBM_File SDBM_File);
}

use AnyDBM_File;
use Storable;
use MLDBM qw(AnyDBM_File Storable);
use POSIX; #for O_CREAT and O_RDWR symbols
use strict;

my %mldbm;
my $mldb_file="mlanydbmdemo.dbm";

tie (%mldbm, 'MLDBM', $mldb_file, O_CREAT|O_RDWR, 0644);

unless (tied %mldbm) {
    print "Error opening $mldb_file: $!\n";
} else {
    if (exists $mldbm{'Created'}) {
        $mldbm{'Created'}=localtime;
    } else {
        $mldbm{'Updated'}=localtime;
    }
    foreach (sort keys %mldbm) {
        print "$_ => $mldbm{$_}\n";
    }
    untie %mldbm;
}
