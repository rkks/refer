#!/usr/bin/perl
# inherit3.plx
use warnings;
use strict;
use Employee3;

my $dilbert = Employee->new (
   surname    => "Dilbert",
   employer   => "Dogbert",
   salary     => "43000"
);

my $boss = $dilbert->employer;
$boss->address("3724 Cubeville");

my $dogbert = Employee->new (
   surname    => "Dogbert",
   employer   => "PHB",
   salary     => $dilbert->salary*2
);

$dilbert->employer($dogbert);

my $phb = $dogbert->employer;
