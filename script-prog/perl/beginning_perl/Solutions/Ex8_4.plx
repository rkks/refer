#!/usr/bin/perl
# ex8_4.plx
use warnings;
use strict;

my @array = (1,1);

sub gen_fib (\@)
{

      my $array_r = shift;
      my $len = $#{$array_r};
      push (@{$array_r} , (@{$array_r}[$len-1]+@{$array_r}[$len]));

}

for( my $i=1; $i<=10; $i++)
{
   gen_fib (@array);
}

print "@array";
