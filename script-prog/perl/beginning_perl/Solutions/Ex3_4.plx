#usr/bin/perl
#Ex3_4.plx
use warnings;
use strict;

my $que_1 = 
   "How many Java programmers does it take to change a light bulb? \n";
my $que_2 = 
   "How many Python programmers does it take to change a light bulb? \n";
my $que_3 = 
   "How many Perl programmers does it take to change a light bulb? \n";
my $que_4 = "How many C programmers does it take to change a light bulb? \n";

my $answer1 = "None. Change it once, and it's the same everywhere. \n\n";
my $answer2 = 
   "One. He just stands below the socket and the world revolves around him.    
   \n";
my $answer3 = 
   "A million. One to change it, the rest to try and do it in fewer lines.  
   \n";
my $answer4 = '"CHANGE?!! \n\n"';

my %jokes = (
             $que_1 => "$answer1",
             $que_2 => "$answer2",
             $que_3 => "$answer3",
             $que_4 => "$answer4"
);

print "$que_1";
sleep 5;
print "$jokes{$que_1}";

print "$que_2";
sleep 5;
print "$jokes{$que_2}";

print "$que_3";
sleep 5;
print "$jokes{$que_3}";

print "$que_4";
sleep 5;
print "$jokes{$que_4}";
