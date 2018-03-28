#!/usr/bin/perl
use strict;
use warnings;
use Net::Pcap qw(pcap_lookupdev pcap_open_live pcap_loop pcap_close);

# 
# this uses the Perl binding for libpcap
# which can be installed on Ubuntu using
#    sudo apt-get install libnet-pcap-perl
# and whose documentation can be viewed by
#    perldoc Net::Pcap 
# 
#

my $err = '';
my $dev = (scalar @ARGV) ? (shift @ARGV) : pcap_lookupdev(\$err);
die "no device to sniff" unless $dev;
print "Sniffing on $dev\n";

# loosely copied from the Net::Pcap documentation
my $pcap = pcap_open_live($dev, 1024, 1, 0, \$err);
pcap_loop($pcap, 10, \&process_packet, "from Net::Pcap docs");
pcap_close($pcap);
sub process_packet {
  my ($user_data, $header, $packet) = @_;
  print "$_ -> $header->{$_}\n" for keys %$header;
  print "\n";
}
