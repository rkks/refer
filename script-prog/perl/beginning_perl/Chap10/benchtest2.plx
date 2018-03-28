#!/usr/bin/perl
# benchtest2.plx
use warnings;
use strict;

use Benchmark;
my $howmany = 10000;

timethese($howmany, {
   line => sub {
      my $file;
      open TEST, "quotes.txt" or die $!;
      while (<TEST>) { $file .= $_ }
      close TEST;
   },
   slurp => sub {
      my $file;
      local undef $/;
      open TEST, "quotes.txt" or die $!;
      $file = <TEST>;
      close TEST;
   },
   join => sub {
      my $file;
      open TEST, "quotes.txt" or die $!;
      $file = join "", <TEST>;
      close TEST;
   }
});
