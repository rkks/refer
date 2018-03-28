#!/usr/bin/perl
# classatr2.plx
use warnings;
use strict;
use Person;

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

print "\nPeople we know:\n";
for my $person(Person->everyone) {
   print $person->forename, " ", $person->surname, "\n";
}
