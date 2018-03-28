#!/usr/bin/perl
# tiescalar2.plx
use warnings;
use strict;
use Autoincrement;

my $count;
tie $count, 'Autoincrement';
print $count, "\n";
print $count, "\n";
print $count, "\n";
print $count, "\n";
$count = "Bye bye!";
print $count, "\n";
print $count, "\n";
print $count, "\n";
print $count, "\n";
