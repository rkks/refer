#!/usr/bin/perl
# tiescalar.plx
use warnings;
use strict;
use Autoincrement;

my $count;
tie $count, 'Autoincrement';

print $count, "\n";
print $count, "\n";
print $count, "\n";
print $count, "\n";
