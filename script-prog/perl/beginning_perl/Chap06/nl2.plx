#!/usr/bin/perl
# nl2.plx
use warnings;
use strict;

my $lineno = 1;

while (<>) {
   if ($current ne $ARGV) {
      $current = $ARGV;
      print "\n\t\tFile: $ARGV\n\n";
      $lineno=1;
   }

   print $lineno++;
   print ": $_";
}
