#!/usr/bin/perl
# bless2.plx
use warnings;
use strict;

my $a = {};

print '$a is a ', ref $a, " reference\n";

bless($a, "Person");
print '$a is a ', ref $a, " reference\n";

bless($a, "Animal::Vertebrate::Mammal");
print '$a is a ', ref $a, " reference\n";
