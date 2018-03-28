#!/usr/bin/perl
# ex8_1.plx
use warnings;
use strict;

my $num_of_seconds = "";
while ($num_of_seconds !~ /^\d+$/) {
   $num_of_seconds = getsecs();
}

my ($hours, $minutes, $seconds) = secs2hms($num_of_seconds);
print "3723 seconds is $hours hours, $minutes minutes and $seconds 

seconds";
print "\n";

sub getsecs {
  print "How many seconds would you like converted? ";
  return <STDIN>;
}
    

sub secs2hms {
   my ($h,$m);
   my $seconds = shift;
   $h = int($seconds/(60*60)); $seconds %= 60*60;
   $m = int($seconds/60);      $seconds %= 60;

   return ($h,$m,$seconds);
}
