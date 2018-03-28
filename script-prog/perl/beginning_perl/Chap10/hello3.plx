#!/usr/bin/perl
# hello3.plx
# Hello World (Deluxe)
use warnings;
use strict;

use Getopt::Std;
my %options;
getopts("vhl:",\%options);

if ($options{v}) {
   print "Hello World, version 3.\n";
   exit;
} elsif ($options{h}) {
   print <<EOF;

$0: Typical Hello World program

Syntax: $0 [–h|-v|-l <language>]

   -h : This help message
   -v : Print version on standard output and exit
   -l : Turn on international language support.
EOF
   exit;
} elsif ($options{l}) {
   if ($options{l} eq "french") {
      print "Bonjour, tout le monde.\n";
   } else {
      die "$0: unsupported language\n";
   }
} else { 
   print "Hello, world.\n";
}
