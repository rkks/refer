#!/usr/bin/perl
# inherit1.plx
use warnings;
use strict;
use Employee1;

my $object = Employee->new (
   surname    => "Galilei",
   forename   => "Galileo",
   address    => "9.81 Pisa Apts.",
   occupation => "bombadier"
);

$object->printletter("You owe me money. Please pay it.");
