#!/usr/bin/perl
# glob.plx
use warnings;
use strict;

my @files = glob("*.txt");
print "Matched *l : @files\n";
