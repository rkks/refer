#!/usr/bin/perl
# nl3.plx
use warnings;
use strict;

my $lineno;
my $current = "";

while (<>) {
   if ($current ne $ARGV) {
      $current = $ARGV;
      print "\n\t\tFile: $ARGV\n\n";
      $lineno=1;
   }

   print $lineno++;
   print ": $_";
}
