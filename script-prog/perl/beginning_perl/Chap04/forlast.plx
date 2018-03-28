#!/usr/bin/perl
# forlast.plx
use warnings;
use strict;

my @array = ( "red", "blue", "STOP THIS NOW", "green");
for (@array) {
	last if $_ eq "STOP THIS NOW";
	print "Today's colour is $_\n";
}
