#!/usr/bin/perl

use strict;
use warnings;

use IPC::Open3;
use Term::ANSIColor;
use Test::Harness::Straps;

my $strap = Test::Harness::Straps->new();

for my $file (@ARGV)
{
    next unless -f $file;

    my $output;

    my $command = $strap->_command_line( $file );
    my $pid     = open3( undef, $output, $output, $command );
    my %results = $strap->analyze( $file, $output );

    print $_->{output} for @{ process_results( $file, \%results ) };
}

sub process_results
{
    my ( $file, $results ) = @_;
    my $count              = 0;

    my @results;
    for my $test ( @{ $results->{details} } )
    {
        $count++;
        next if $test->{ok};

        push @results =>
        {
            test   => $test,
            output => create_test_result(
                $test->{ok}, $count, @{$test}{qw( name reason diagnostics )}
            )
        };
    }

    return \@results;
}

sub create_test_result
{
    my ( $ok, $number, $name, $reason, $diag ) = @_;

    $ok       = $ok ? 'ok' : 'not ok';
    $reason ||= '';
    $reason   = " ($reason)" if $reason;
    $diag   ||= '';

    return color( 'bold red' ) .
           sprintf "%6s %4d %s%s\n%s\n", $ok, $number, $name, $reason, 
           color( 'clear yellow' ) . $diag . color( 'reset' );
}
