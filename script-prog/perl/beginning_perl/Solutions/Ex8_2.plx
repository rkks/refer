#!/usr/bin/perl
# ex8_2.plx
use warnings;
use strict;

my $count = 0;

sub sub1 {
   ++$count;   
   print "This is subroutine call $count\n";
   sub2() unless $count==300;
   return;
}

sub sub2 {
   ++$count;   
   print "This is subroutine call $count\n";
   sub3() unless $count==300;
   return;
}

sub sub3 {
   ++$count;   
   print "This is subroutine call $count\n";
   sub1 unless $count==300;
   return;
}

sub1;
