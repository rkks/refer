#!/usr/bin/perl 
#cgihello.plx
use strict;
use warnings;

print "Content-type: text/plain\n\n";
print "Hello CGI World!\n";
print "You're calling from $ENV{REMOTE_HOST}\n";


