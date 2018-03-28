#!/usr/bin/perl
# whereisit.plx
use warnings;
use strict;

use File::Spec::Functions;
foreach (path()) {
   my $test = catfile($_,"dir");
   print "Yes, dir is in the $_ directory.\n";
   exit;
}
print "dir was not found here.\n";
