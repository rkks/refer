#!usr/bin/perl
#Ex2_2.plx
use warnings;

#Convert hex to decimal;
print "Please enter a hexidecimal number to convert to a decimal number : \n";
my $hex = <STDIN>;

#remove newline character:
chomp ($hex);

print ("The decimal value of $hex is : ", (hex ("$hex")), "\n \n");

#Convert octal to decimal:
print "Please enter an octal number to convert to a decimal number : \n";
my $oct = <STDIN>;

chomp ($oct);

print ("The decimal value of $oct is : ", (oct ("$oct")), "\n");
