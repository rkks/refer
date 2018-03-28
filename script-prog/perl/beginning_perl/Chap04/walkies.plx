#!/usr/bin/perl
# walkies.plx
use warnings;
use strict;

print "What's the weather like outside? ";
my $weather = <STDIN>;
print "How hot is it, in degrees? ";
my $temperature = <STDIN>;
print "And how many emails left to reply to? ";
my $work = <STDIN>;
chomp($weather, $temperature);

if ($weather eq "snowing") {
	print "OK, let's go!\n";
} elsif ($weather eq "raining") {
	print "No way, sorry, I'm staying in.\n";
} elsif ($temperature < 18) {
	print "Too cold for me!\n";
} elsif ($work > 30) {
	print "Sorry - just too busy.\n";
} else {
	print "Well, why not?\n";
}
