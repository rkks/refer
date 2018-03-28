#!/usr/bin/perl
# Ex6_1.plx
use warnings;
use strict;

print "What string would you like to search for?";
chomp (my $query = <STDIN>);

opendir DH, "." or die "Couldn't open the current directory: $!";
while ($_ = readdir(DH)) {
        next if $_ eq "." or $_ eq "..";

        eval {open FH, $_ or die "Couldn't open file $_";};
        my $file = $_;
        while (<FH>) {
         if (/$query/) {
              print "Found \"$query\" in file ";
              print $file, "." x (30-length($file));
              print "d" if -d $file;
              print "r" if -r _;
              print "w" if -w _;
              print "x" if -x _;
              print "o" if -o _;
              print "." x 10;
              print -s _ if -r _ and -f _;
              print "\n";

         }
   }
}
