package TraceGlobals;

use strict;
use warnings;

use Runops::Trace \&trace_globals;

my %globals;

sub trace_globals
{
    return unless $_[0]->isa( 'B::SVOP' ) && $_[0]->name() eq 'gv';
    my $gv   = shift->gv();
    my $data = $globals{ $gv->SAFENAME() } ||= {};
    my $key  = $gv->FILE() . ':' . $gv->LINE();
    $data->{$key}++;
}

END
{
    Runops::Trace->unimport();

    for my $gv ( sort keys %globals )
    {
        my $gv_data = $globals{ $gv };
        my @counts  = keys %$gv_data;

        for my $line ( sort { $gv_data->{$b} <=> $gv_data->{$a} } @counts)
        {
            printf "%04d %s %-> s\n", $gv_data->{$line}, $gv, $line;

        }
    }
}

1;
