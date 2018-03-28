#!/usr/bin/perl
# sumsquare.plx
use warnings;
use strict;
sub sum_of_two_squares ($$);

my ($first, $second) = @ARGV;
print "The sum of the squares of $first and $second is ";
print sum_of_two_squares($first, $second),"\n";

# print sum_of_two_squares($first, $second, 0),"\n";

sub sum_of_two_squares ($$) {
   my ($a,$b) = (shift, shift);
   return $a**2+$b**2;
}
