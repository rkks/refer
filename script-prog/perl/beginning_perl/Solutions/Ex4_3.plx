#!usr/bin/perl
#Ex4_3.plx
use warnings;
use strict;

print "To what number would you like to calculate the primes?", "\n";
my $n = <STDIN>;
my $i = 3;
print "1 2 ";

# We will keep executing our search until our 
# number ($i) reaches the given value:

OUTER: while ($i <= $n){

   #Each time we iterate, we must begin division by 2:

   my $num = 2;
  
   #Do checks to see if our value $i is prime:            
   for (1 .. $i){        
      if (($i % $num == 0) and ($i != $num)){

         #If it is not, then check the next number:
         $i++;
         next OUTER;
      }

      if ($num > sqrt($i)) {
         print "$i ";
         $i++;
         next OUTER;
      }
      $num++;
   }
}
