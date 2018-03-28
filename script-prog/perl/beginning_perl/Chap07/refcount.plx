#!/usr/bin/perl
# refcount.plx
use warnings;
use strict;

my $ref;
{
   my @array = (1, 2, 3);
   $ref = \@array;
   my $ref2 = \@array;
   $ref2 = "Hello!";
} 
undef $ref;
