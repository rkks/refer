#!/usr/bin/perl

use strict;
use warnings;

use lib 'lib';
use SourceCarp fatal => 1, warnings => 1;

# throw warning
open my $fh, '<', '/no/file';
print $fh "Hello!";

# report from subroutine
report_with_level();

sub report_with_level
{
    SourceCarp::report( "report caller, not self\n", 2 );
}

# throw error
die "Oops!";
