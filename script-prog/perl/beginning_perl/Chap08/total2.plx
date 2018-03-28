#!/usr/bin/perl
# total2.plx
use warnings;
use strict;

my $total      = total(111, 107, 105, 114, 69);
my $sum_of_100 = total(1...100);

sub total {
   my $total = 0;
   $total += $_ for @_;
   $total;
}

