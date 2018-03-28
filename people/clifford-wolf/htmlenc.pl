#!/usr/bin/perl
#
#  htmlenc.pl - Encode _ALL_ special characters as HTML entities
#  Copyright (C) 2007  Clifford Wolf <clifford@clifford.at>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or

use Getopt::Long;

my $first = 1;

my $tabs2spaces = 0;
my $spaces2nbsp = 0;
my $insertbr = 0;

if (!GetOptions('tabs2spaces' => \$tabs2spaces,
		'spaces2nbsp' => \$spaces2nbsp,
		'insertbr' => \$insertbr)) {
        print STDERR "Usage: $0 [ --tabs2spaces ] [ --spaces2nbsp ] [ --insertbr ]\n";
        exit 1;
}

while (<>)
{
	chomp if $insertbr;
	s/([^a-zA-Z0-9 \t\n])/"&#".ord($1).";"/eg;
	s/(\t)/$tabs2spaces ? "        " : "&#".ord($1).";"/eg;
	s/ /&nbsp;/g if $spaces2nbsp;
	print "<br/>" if !$first and $insertbr;
	print $_;
	$first = 0;
}

print "\n" if $insertbr;

