#!/usr/bin/perl
# bless1.plx
use warnings;
use strict;

my $a = {};

print '$a is a ', ref $a, " reference\n";

bless($a, "Person");

print '$a is a ', ref $a, " reference\n";
