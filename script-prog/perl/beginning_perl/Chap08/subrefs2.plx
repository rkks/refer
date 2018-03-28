#!/usr/bin/perl
# subrefs2.plx
use warnings;
use strict;
sub increment (\$);

my $a = 5;
increment($a);
print $a;

sub increment (\$) {
   my $reference = shift;
   $$reference++;
}
