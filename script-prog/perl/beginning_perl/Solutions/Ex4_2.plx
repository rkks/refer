#!/usr/bin/perl
#Ex4_2.plx
use warnings;
use strict;

my $target = 73;
print "Guess my number!\n";
print "Enter your guess: ";
my $guess = <STDIN>;



while ($guess != $target) {
    
   if ($guess > $target){
      print "Your number is bigger than my number, guess again: \n";
   }  

   if ($guess < $target){
      print "Your number is less than my number, guess again: \n";
   }

   $guess = <STDIN>;
}

print "That's it! You guessed correctly!\n";
