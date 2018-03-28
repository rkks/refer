package Devel::Presolve;

use strict;
use warnings;

my @track;

BEGIN { unshift @INC, \&resolve_path }

sub resolve_path
{
    my ($code, $module) = @_;
    push @track, $module;
    return;
}

INIT
{
    print "BEGIN\n{\n";

    for my $tracked (@track)
    {
        print "\trequire( \$INC{'$tracked'} = '$INC{$tracked}' );\n";
    }

    print "}\n1;\n";
    exit;
}

1;
