#!/usr/bin/perl
# next.plx
use strict;
use warnings;

my @array = (8, 3, 0, 2, 12, 0);
for (@array) {
	if ($_ == 0) {
		print "Skipping zero element.\n";
		next;
	}
	print "48 over $_ is ", 48/$_, "\n";
}
