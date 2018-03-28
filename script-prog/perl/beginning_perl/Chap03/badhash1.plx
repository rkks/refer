#!/usr/bin/perl
#badhash1.plx
use warnings;
use strict;

my %where=(
        Gary     => "Dallas",
        Lucy     => "Exeter",
        Ian      => "Reading",
        Samantha => "Oregon"
);


delete $where{Lucy};
print "Lucy lives in $where{Lucy}\n";