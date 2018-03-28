#!/usr/bin/perl
#
# usage: ./dial.pl 1234567
#        plays the DTMF tones for the digits
#
# by Troy D. Hanson, July, 2014.
#
use strict;
use warnings;
my $PROG = "./sines";
die "run make first" unless -f $PROG;
die "enter a phone number" unless @ARGV;
my $number = shift @ARGV;
my @digits = split //, $number;

# DTMF tones. two frequences for each key
my %tones;
push @{$tones{ 1}}, 1209, 697 ;
push @{$tones{ 2}}, 1336, 697 ;
push @{$tones{ 3}}, 1477, 697 ;
push @{$tones{ 4}}, 1209, 770 ;
push @{$tones{ 5}}, 1336, 770 ;
push @{$tones{ 6}}, 1477, 770 ;
push @{$tones{ 7}}, 1209, 852 ;
push @{$tones{ 8}}, 1336, 852 ;
push @{$tones{ 9}}, 1477, 852 ;
push @{$tones{"*"}},1209, 941 ;
push @{$tones{ 0}}, 1336, 941 ;
push @{$tones{"#"}},1477, 941 ;

system($PROG, "-s", 1, @{$tones{$_}}) for @digits;
