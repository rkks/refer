#!/usr/bin/perl
# hoover.plx
use strict;
use warnings;

use File::Find;
find(\&cleanup, "/");

sub cleanup {
   # Not been accessed in six months?
   if (-A > 180) {
      print "Deleting old file $_\n";
      unlink $_ or print "oops, couldn't delete $_: $!\n";
      return;
   }
   open (FH, $_) or die "Couldn't open $_: $!\n";
   for (1..5) { # You've got five chances.
      my $line = <FH>;
      if ($line =~ /Perl|Simon|important/i) {
         # Spare it.
         return;
      }
   }
   print "Deleting unimportant file $_\n";
   unlink $_ or print "oops, couldn't delete $_: $!\n";
}
