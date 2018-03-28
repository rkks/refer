#!/usr/bin/perl
# hellolong.plx
# Hello World (Deluxe) - with long flags
use warnings;
use strict;

use Getopt::Long;
my %options;
GetOptions(\%options, "language:s", "help", "version");

if ($options{version}) {
   print "Hello World, version 3.\n";
   exit;
} elsif ($options{help}) {
   print <<EOF;

$0: Typical Hello World program

Syntax: $0 [–-help|--version|--language=<language>]

   --help     : This help message
   --version  : Print version on standard output and exit
   --language : Turn on international language support.
EOF
   exit;
} elsif ($options{language}) {
   if ($options{language} eq "french") {
      print "Bonjour, tout le monde.\n";
   } else {
      die "$0: unsupported language\n";
   }
} else { 
   print "Hello, world.\n";
}
