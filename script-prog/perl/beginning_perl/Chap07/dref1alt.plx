#!/usr/bin/perl
# dref1alt.plx
use warnings;
use strict;

my @array   = (1, 2, 3, 4, 5);
my $array_r = \@array;

print "This is our dereferenced array: @$array_r\n";
for (@$array_r) {
   print "An element: $_\n";
}
print "The highest element is number $#$array_r\n";
print "This is what our reference looks like: $array_r\n";
