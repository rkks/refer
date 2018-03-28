#!/usr/bin/perl
# benchtest.plx
use warnings;
use strict;

use Benchmark;
my $howmany = 10000;
my $what    = q/my $j=1; for (1..100) {$j*=$_}/;

timethis($howmany, $what);
