#!/usr/bin/perl
# sort3.plx
use warnings;
use strict;

my $numeric = 0;
my $input = shift;
if (defined $input and $input eq "-n") {
   $numeric = 1;
   $input = shift;
}
my $output = shift;

if (defined $input) {
   open INPUT, $input or die "Couldn't open file $input: $!\n";
} else {
   *INPUT = *STDIN;
}

if (defined $output) {
   open OUTPUT, ">$output" or die "Couldn't open file $input: $!\n";
} else {
   *OUTPUT = *STDOUT;
}

my @file = <INPUT>;
if ($numeric) {
   @file = sort { $a <=> $b } @file;
} else {
   @file = sort @file;
}

print OUTPUT @file;


