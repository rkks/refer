#!/usr/bin/perl
# biglist.plx
use warnings;
use strict;
use File::Find;
find ( \&callback, "/") ; # Warning: Lists EVERY FILE ON THE DISK!

sub callback {
   print $File::Find::name, "\n";
}
