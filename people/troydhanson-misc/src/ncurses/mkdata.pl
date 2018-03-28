#!/usr/bin/perl
use strict;
use warnings;
local $|;
open FIFO, ">/tmp/slidogram.fifo" or die;
select FIFO; $| = 1; # unbuffered
for(0..99) {
  my $r = int rand 100;
  print FIFO ",$r";
  sleep(1); # if ((int rand 10) == 0);
}
close FIFO;
