#!/usr/bin/perl
# match3.plx
use warnings;
use strict;

my $test1 = "The dog is in the kennel";
my $test2 = "The sheepdog is in the field";

if ($test1 =~ / dog/) {
	print "This dog's at home.\n";
}	

if ($test2 =~ / dog/) {
	print "This dog's at work.\n";
}
