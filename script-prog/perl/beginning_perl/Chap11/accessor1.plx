#!/usr/bin/perl
# accessor1.plx
use Person4;

my $object = Person->new (
   surname    => "Galilei",
   forename   => "Galileo",
   address    => "9.81 Pisa Apts.",
   occupation => "bombadier"
);
print "This person's surname: ", $object->surname, "\n";
