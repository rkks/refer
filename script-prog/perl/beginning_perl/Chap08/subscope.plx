#!/usr/bin/perl
# subscope.plx
use warnings;
use strict;

first(1,2,3);

sub first {
   print "In first, arguments are @_\n";
   second(4,5,6);
   print "Back in first, arguments are @_\n";

}
sub second {
   print "In second, arguments are @_\n";
}
