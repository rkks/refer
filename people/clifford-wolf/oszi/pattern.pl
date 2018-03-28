#!/usr/bin/perl -w

use strict;
use English;

if (defined $ARGV[0] and $ARGV[0] eq "am") {
	my $i = 100;
	while (1) {
		printf "%d %d\n", -$i, -$i;
		printf "%d %d\n", -$i, -$i;
		printf "%d %d\n", +$i, -$i;
		printf "%d %d\n", +$i, -$i;
		printf "%d %d\n", -$i, +$i;
		printf "%d %d\n", -$i, +$i;
		printf "%d %d\n", +$i, +$i;
		printf "%d %d\n", +$i, +$i;
		$i = $i > 1000 ? 100 : $i + 100;
	}
} elsif (defined $ARGV[0] and $ARGV[0] eq "fsweep") {
	my ($r, $i, $phase) = (1, 0, 0);
	while (1) {
		$phase = ($phase+1) % int(3.14*2*100000);
		my $phi = (sin($phase / 100000) + 1) / 2;
		($r, $i) = (
			$r*cos($phi) + $i*sin($phi),
			-$r*sin($phi) + $i*cos($phi)
		);
		my $abs = sqrt($r*$r + $i*$i);
		($r, $i) = ($r/$abs, $i/$abs);
		printf "%d %d\n", $r*2500, $i*2500;
	}
} else {
	my $i = 0;
	while (1) {
		printf "%d %d\n", cos($i)*2500, sin($i*0.99)*2000;
		$i += 0.01;
	}
}

