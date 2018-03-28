#!/usr/bin/perl
# nomatch.plx
use warnings;
use strict;

my $gibson =
   "Nobody wants to hurt you... 'cept, I do hurt people sometimes, Case.";

if ($gibson !~ /fish/) {
   print "There are no fish in William Gibson.\n";
}
