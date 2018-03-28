#!/usr/bin/perl -w

use English;
use strict;

my $target = $ARGV[0];

while (1)
{
	if (defined $target) {
		last if $target eq "latex";
		last if $target eq "html";
	}
	print STDERR "Usage: $0 { latex | html }\n";
	exit 1;
}

sub fixuml($)
{
	my $s = $_[0];
	return $s if $s =~ /.../;
	if ($target eq "latex") {
		return "\\\"u" if $s eq "ue";
		return "\\\"U" if $s eq "Ue";
		return "\\\"a" if $s eq "ae";
		return "\\\"A" if $s eq "Ae";
		return "\\\"o" if $s eq "oe";
		return "\\\"O" if $s eq "Oe";
	}
	if ($target eq "html") {
		return "&uuml;" if $s eq "ue";
		return "&Uuml;" if $s eq "Ue";
		return "&auml;" if $s eq "ae";
		return "&Auml;" if $s eq "Ae";
		return "&ouml;" if $s eq "oe";
		return "&Ouml;" if $s eq "Oe";
	}
	die "Can't handle sequence '$s' for target '$target'.";
}

while (<STDIN>) {
	s/(\bUei?|\bAei?|\bOei?|[aeq]?uei?|aei?|oei?)/fixuml($1)/ge;
	print $_;
}

exit 0;

