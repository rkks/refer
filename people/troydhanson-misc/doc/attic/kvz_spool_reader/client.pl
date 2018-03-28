#!/usr/bin/perl
use strict;
use warnings;
use Data::Dumper;
use JSON;
use ZeroMQ qw/:all/;

# install ZeroMQ using "cpan ZeroMQ"

my $ctx = ZeroMQ::Context->new;
my $sock = $ctx->socket(ZMQ_REQ);
$sock->connect("tcp://127.0.0.1:3333");

$sock->send("stats");
my $d = $sock->recv()->data();
print Dumper from_json($d), "\n";
