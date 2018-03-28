#!/usr/bin/perl
# hello2.plx
use warnings;
use strict;

sub version {
   print "Beginning Perl's \"Hello, world.\" version 2.0\n";
}

my $option = shift;
version if $option eq "-v" or $option eq "--version";
print "Hello, world.\n";
