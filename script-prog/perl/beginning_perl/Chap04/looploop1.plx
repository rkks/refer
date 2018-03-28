#!/usr/bin/perl
# looploop1.plx
use warnings;
use strict;
my @getout = qw(quit leave stop finish);

while (<STDIN>) {
	chomp;
	for my $check (@getout) {
		last if $check eq $_;
	}
	print "Hey, you said $_\n";
}
