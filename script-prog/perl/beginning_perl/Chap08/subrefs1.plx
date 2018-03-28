#!/usr/bin/perl
use warnings;
use strict;

my $a = 5;
increment(\$a);
print $a;

sub increment {
   my $reference = shift;
   $$reference++;
}
