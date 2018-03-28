#!/usr/bin/perl
# ex5_03.plx
use warnings;
use strict;

my @vars;
$_ = '1: A silly sentence (495,a) *BUT* one which will be useful. (3)';

print "Enter some text to find: ";
my $pattern = <STDIN>;
chomp($pattern);

if (@vars = /$pattern/) {
   print "The text matches the pattern '$pattern'.\n";
   foreach (@vars) {
      print "Group: $_\n";
   }
} else {
   print "'$pattern' was not found.\n";
}
