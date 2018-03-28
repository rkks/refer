#!/usr/bin/perl 
#Ex2_1.plx
use warnings;
use strict;

print "Currency converter\n\nPlease enter the Yen to pound exchange rate: ";
my $yen = <STDIN>;

#Prompt user to input amounts, and do the conversions:
print "Now please enter your first amount: ";
my $first = <STDIN>;
my $a = ($first/$yen);
print "Enter your second amount: ";
my $second = <STDIN>;
my $b = ($second/$yen);
print "Enter your third amount: ";
my $third = <STDIN>;
my $c = ($third/$yen);

#turn the results into values with no more than 2 decimal places, and rounded #to 2 places too.
my $mod1 = (int (100*$a)/100);
my $mod2 = (int (100*$b)/100);
my $mod3 = (int (100*$c)/100);

#Get rid of trailing newline character:
chomp ($first, $second, $third);

#print out answers with two decimal place accuracy:
printf ("$first Yen is %.2f pounds\n", $mod1);
printf ("$second Yen is %.2f pounds\n", $mod2);
printf ("$third Yen is %.2f pounds\n", $mod3);
