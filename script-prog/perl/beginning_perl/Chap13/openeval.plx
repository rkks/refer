#!/usr/bin/perl
#openeval.plx
use warnings;
use strict;
use POSIX;
use SDBM_File;
use strict;

my %dbm;
my $db_file="demo.dbm";

eval {
tie %dbm, 'SDBM_File', $db_file, O_RDWR, 0;
};

if ($@) {
	print "Error tieing to $db_file: $@\n";
} elsif (!tied(%dbm)) {
print "Error opening $db_file: $!\n";
}
