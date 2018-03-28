#!/usr/bin/perl
# deref2.plx
use warnings;
use strict;

my @band = qw(Crosby Stills Nash Young);
my $ref  = \@band;
for (0..3) {
   print "Array    : ", $band[$_]  , "\n";
   print "Reference: ", ${$ref}[$_], "\n";
}
