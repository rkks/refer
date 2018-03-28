#!/usr/bin/perl
use strict;
use warnings;
use IO::Socket;
use Getopt::Long;

sub usage {
  print "usage: $0 [-v] [-s <ip>] [-p <port>]\n";
  exit(-1);
}

our $server = "127.0.0.1";
our $port = 6454;
our $verbose;
our $help;
our $msg = "Art-Net\0" . # first 8 bytes
           "\x00\x50"  . # opcode = 0x5000 (little endian)
           "\x00\x0e"  . # version=14 (0x000e) (big endian)
           "\0"        . # sequence
           "\0"        . # physical
           "\0\0"      . # port-address (net/subnet/universe)
           "\x00\x30"  ; # length 48 (big endian)
                         # followed by RGB bytes
usage unless GetOptions("port=i"   => \$port,
                        "server=s" => \$server,
                        "verbose+" => \$verbose,
                        "help"     => \$help);
usage if $help;
usage unless $port;
my $sock = IO::Socket::INET->new(PeerPort => $port, PeerAddr => $server, 
                                 Proto => "udp", ) or die "socket: $!\n";
while(1) {
 my $rgb = "";
 my $i = 0;
 $rgb .= pack "C", int(rand(256)) while ($i++ < 48);
 $sock->send($msg.$rgb);
 sleep 1;
}
