#!/usr/bin/perl
# summary.plx
use warnings;
use strict;

$/ = "";
my $counter = 1;

while (<>) {
   print $counter++, ":";
   print ((split /\n/, $_)[0]);
   print "\n";
}
