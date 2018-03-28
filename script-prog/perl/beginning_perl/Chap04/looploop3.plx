#!/usr/bin/perl
# looploop3.plx
use warnings;
use strict;
my @getout = qw(quit leave stop finish);

OUTER: while (<STDIN>) {
	chomp;
	INNER: for my $check (@getout) {
		last OUTER if $check eq $_;
	}
	print "Hey, you said $_\n";
}
