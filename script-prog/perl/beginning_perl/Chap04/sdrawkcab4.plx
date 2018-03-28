#!/usr/bin/perl
# sdrawkcab4.plx
use warnings;
use strict;

while (<STDIN>) {
	chomp;
	last unless $_;
	my $sdrawkcab = reverse $_;
	print "$sdrawkcab\n";
}
# and now we can carry on with something else...
