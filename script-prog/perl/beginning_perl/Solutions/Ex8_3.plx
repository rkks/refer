#!/usr/bin/perl
# ex8_3.plx
use warnings;
use strict;

sub sub1 {
   my ($count, $limit) = @_;   
   print "This is subroutine call ", ++$count, "\n";
   sub2($count, $limit) unless $count==$limit;
   return;
}

sub sub2 {
   my ($count, $limit) = @_;   
   print "This is subroutine call ", ++$count, "\n";
   sub3($count, $limit) unless $count==$limit;
   return;
}

sub sub3 {
   my ($count, $limit) = @_;   
   print "This is subroutine call ", ++$count, "\n";
   sub1($count, $limit) unless $count==$limit;
   return;
}

sub1(0, 300);
