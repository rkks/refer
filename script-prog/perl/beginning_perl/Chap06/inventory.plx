#!/usr/bin/perl
# inventory.plx
use warnings;
use strict;

my %inventory;
print "Enter individual items, followed by a new line.\n";
print "Enter a blank line to finish.\n";
while (1) {
   my $item = <STDIN>;
   chomp $item;
   last unless $item;
   $inventory{lc $item}++;
}

open(SORT, "| perl sort3.plx") or *SORT = *STDOUT;
select *SORT;
while (my ($item, $quantity) = each %inventory) {
   if ($quantity > 1) {
      $item =~ s/^(\w+)\b/$1s/ unless $item =~ /^\w+s\b/;
   }
   print "$item: $quantity\n";
}
