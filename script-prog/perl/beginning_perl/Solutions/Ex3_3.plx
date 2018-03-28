#!usr/bin/perl
#Ex3_3.plx
use warnings;

#Create hash with important numbers:

my %numbers = (
               mum => "555-1111",
               dad => "555-2222",
               bro => "555-3333",
               sis => "555-4444"
        );


#Get persons name:

print "Please enter a name : \n";
my $Name = <STDIN>;
chomp ($Name);

#Find and print the name's number
print "$Name","'s number is $numbers{$Name}", "\n";
