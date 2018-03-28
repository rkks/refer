#!perl

use strict;
use warnings;

use HTTP::Recorder;
use HTTP::Proxy;

my $agent = HTTP::Recorder->new( file => "mech2.t", showwindow => 1 );

my $proxy = HTTP::Proxy->new(
    port  => 4567,
    agent => $agent,
);

$proxy->start();
