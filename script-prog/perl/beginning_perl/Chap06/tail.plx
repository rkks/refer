#!/usr/bin/perl
# tail.plx
use warnings;
use strict;

open FILE, "gettysburg.txt" or die $!;
my @last5;

while (<FILE>) {
   push @last5, $_; # Add to the end
   shift @last5 if @last5 > 5; # Take from the beginning
}

print "Last five lines:\n", @last5;
