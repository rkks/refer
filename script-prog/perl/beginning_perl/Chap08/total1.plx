#!/usr/bin/perl
# total1.plx
use warnings;
use strict;

total(111, 107, 105, 114, 69);
total(1...100);

sub total {
my $total=0;
my @args = @_;
foreach my $element (@args) {
   $total = $total+$element;
}

   print "The total is $total\n";
}
