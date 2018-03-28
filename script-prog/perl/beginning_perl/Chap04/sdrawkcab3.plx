#!/usr/bin/perl
# sdrawkcab3.plx
use warnings;
use strict;

my $stopnow = 0;
until ($stopnow) {
	$_ = <STDIN>;
	chomp;
if ($_) {
		my $sdrawkcab = reverse $_;
		print "$sdrawkcab\n";
	} else {
		$stopnow = 1;
	}
}
print "!enod llA\n";
