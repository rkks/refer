#!/usr/bin/perl
# runtime.plx
use strict;
use warnings;
my $x = 10;              # Line 5
$_ = "alpha";
{
   my $x = 20;
   local $_ = "beta";
   somesub();          # Line 10
}
somesub();

sub somesub {
   print "\$x is $x\n";
   print "\$_ is $_\n";
}
