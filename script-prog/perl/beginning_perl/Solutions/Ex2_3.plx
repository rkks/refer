#!usr/bin/perl
#Ex2_3.plx
use warnings;

print "Please enter the value(less than 256) you wish to be converted into binary : \n";
my $bin = <STDIN>;

chomp ($bin);

print "The binary value of $bin is : ", "\n";

#Use the bitwise and operator to determine the binary value:

print ((128&$bin)/128);
print ((64&$bin)/64);
print ((32&$bin)/32);
print ((16&$bin)/16);
print ((8&$bin)/8);
print ((4&$bin)/4);
print ((2&$bin)/2);
print ((1&$bin)/1);
print "\n";
