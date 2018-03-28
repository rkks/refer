#!/usr/bin/perl
# seconds.plx
use warnings;
use strict;
my ($hours, $minutes, $seconds) = secs2hms(3723);
print "3723 seconds is $hours hours, $minutes minutes and $seconds seconds";
print "\n";

sub secs2hms {
   my ($h,$m);
   my $seconds = shift;
   $h = int($seconds/(60*60)); $seconds %= 60*60;
   $m = int($seconds/60);      $seconds %= 60;
   return ($h,$m,$seconds);
}
