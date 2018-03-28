#!/usr/bin/perl
#Ex4_1.plx
use warnings;
use strict;

my ($value, $from, $to, $rate, %rates);
%rates = (
    pounds          => 1,
    dollars         => 1.6,
    marks           => 3.0,
    "french francs" => 10.0,
    yen             => 174.8,
    "swiss francs"  => 2.43,
    drachma         => 492.3,
    euro            => 1.5
);

print "Enter your starting currency: ";
$from = <STDIN>;
print "Enter your target currency: ";
$to = <STDIN>;
print "Enter your amount: ";
$value = <STDIN>;

chomp($from,$to,$value);

#If this currency does not exist, then execute this subroutine:
while (not exists $rates{$to}) {

   print "I don't know anything about $to as a currency\n";
   print "Re-enter your target currency: ";
   $to = <STDIN>;
   chomp($to);
}

while (not exists $rates{$from}) {
    
   print "I don't know anything about $from as a currency\n";
   print "Re-enter your starting currency: ";
   $from = <STDIN>;
   chomp($from);
}

$rate = $rates{$to} / $rates{$from};

print "$value $from is ",$value*$rate," $to.\n";
