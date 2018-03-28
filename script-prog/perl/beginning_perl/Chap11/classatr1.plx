#!/usr/bin/perl
# classatr1.plx
use warnings;
use strict;
use Person6;

print "In the beginning: ", Person->headcount, "\n";
my $object = Person->new (
   surname    => "Gallelei",
   forename   => "Galleleo",
   address    => "9.81 Pisa Apts.",
   occupation => "bombadier"
);
print "Population now: ", Person->headcount, "\n";

my $object2 = Person->new (
   surname    => "Einstein",
   forename   => "Albert",
   address    => "9E16, Relativity Drive",
   occupation => "Plumber"
);
print "Population now: ", Person->headcount, "\n";
