#!/usr/bin/perl
# sdrawkcab1.plx
use warnings;
use strict;

while (<STDIN>) {
	chomp;
	die "!enod llA\n" unless $_;
	my $sdrawkcab = reverse $_;
	print "$sdrawkcab\n";
}
