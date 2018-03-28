#!/usr/bin/perl
# sdrawkcab2.plx
use warnings;
use strict;

do {
	$_ = <STDIN>;
	chomp;
	my $sdrawkcab = reverse $_;
	print "$sdrawkcab\n";
} while ($_);
print "!enod llA\n";
