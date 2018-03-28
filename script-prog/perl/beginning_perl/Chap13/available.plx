#!/usr/bin/perl
#available.plx
use warnings;
use strict;
use DBI;

print "Available DBI Drivers and Data Sources:\n\n";
my @drivers=DBI->available_drivers('quiet');
my @sources;
foreach my $driver (@drivers) {
    print "$driver\n";
    @sources=eval { DBI->data_sources($driver) };
    if ($@) {
        print "\tError: ",substr($@,0,60),"\n";
    } elsif (@sources) {
        foreach (@sources) {
            print "\t$_\n";
        }
    } else {
        print "\tNo known data sources\n";
    }
}
