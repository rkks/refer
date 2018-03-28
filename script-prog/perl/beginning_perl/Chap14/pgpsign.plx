#!/usr/bin/perl
# pgpsign.plx
use warnings;
use strict;
use PGP::Pipe;

my $pgp = PGP::new;
print "What file do you want to sign? ";
my $file = <>;
chomp $file; die $! unless -e $file;

print "Enter your password: ";
use Term::Readkey;
ReadMode("noecho");
my $password = ReadLine(0);
ReadMode("echo");

print $pgp->Sign(File => $file, Password => $password, Armor => 1);
