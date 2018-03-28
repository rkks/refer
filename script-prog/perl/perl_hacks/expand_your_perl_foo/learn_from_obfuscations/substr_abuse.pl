#!/usr/bin/perl                              # how to (ab)use substr
use warnings;
use strict;

my $pi='3.14159210535152623346475240375062163750446240333543375062';

     substr      ($^X,0)=
       substr    ($pi,-6);map{
         substr  ($^X,$.++,1)=chr(
          substr ($pi,21,2)+
          substr ($pi,$_,2))}(12,28,-18,-6,-10,14);map{$^O=$"x(
         substr  ($pi,-5,2));
       substr    ($^O,sin(++$a/8)*32+
     substr      ($pi,-2)/2+1,1)=$_;
   substr        ($^O,sin($a/4)*(
 substr          ($pi,2,2))+
substr           ($pi,-7,-5)-1,1)=$_;print"$^O$/";eval($^X.('$b,'x3).
substr           ($pi,-3,1).'.'.
 substr          ($pi,9,2));}(map{chr($_+
   substr        ($pi,21,2))}(
     substr      ($pi,8)x6)=~/../g);
