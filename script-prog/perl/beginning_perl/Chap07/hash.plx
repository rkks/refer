#!/usr/bin/perl
# hash.plx
use warnings;
use strict;

my %hash = ( 
   1 => "January",    2 => "February", 3 => "March",     4 => "April", 
   5 => "May",        6 => "June",     7 => "July",      8 => "August", 
   9 => "September", 10 => "October", 11 => "November", 12 => "December"
);

my $href = \%hash;
for (keys %{$href}) {
   print "Key: ", $_, "\t";
   print "Hash: ",$hash{$_}, "\t";
   print "Ref: ",${$href}{$_}, "\n";
}
