#/usr/bin/perl
# bigfloat.plx
use strict;
use warnings;
use Math::BigFloat;

my $bignum = Math::BigFloat->new(1);
print "Without BigFloat : ", 1/3, "\n";
print "With BigFloat : ", $bignum/3, "\n";

print "With BigFloat : ", $bignum->fdiv(3), "\n";