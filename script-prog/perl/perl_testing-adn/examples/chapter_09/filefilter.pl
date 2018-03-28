#!perl

use strict;
use warnings;

main( @ARGV ) unless caller();

sub main
{
    die "Usage:\n$0 <command> [file_pattern]\n" unless @_;

    my $command     = shift;
    my $command_sub = main->can( "cmd_$command" );
    die "Unknown command '$command'\n" unless $command_sub;

    print join( "\n", $command_sub->( @_ ) );
}

sub sort_by_time
{
    map  { $_->[0] }
    sort { $a->[1] <=> $b->[1] }
    map  { [ $_, -M $_ ] } @_
}

sub cmd_latest
{
    (sort_by_time( @_ ) )[0];
}

sub cmd_dirs
{
    grep { -d $_ } @_;
}

# return true
1;
