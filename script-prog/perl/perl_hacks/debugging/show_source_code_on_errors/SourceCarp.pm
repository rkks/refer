package SourceCarp;

use strict;
use warnings;

sub import
{
    my ($class, %args) = @_;
    $SIG{__DIE__}      = sub { report( shift, 2 ); exit } if $args{fatal};
    $SIG{__WARN__}     = \&report                         if $args{warnings};
}

sub report
{
    my ($message, $level)  = @_;
    $level               ||= 1;
    my ($filename, $line)  = ( caller( $level - 1 ) )[1, 2];
    warn $message, show_source( $filename, $line );
}

sub show_source
{
    my ($filename, $line) = @_;
    return '' unless open( my $fh, $filename );

    my $start = $line - 2;
    my $end   = $line + 2;

    local $.;
    my @text;
    while (<$fh>)
    {
        next unless $. >= $start;
        last if     $. >  $end;
        my $highlight   = $. == $line ? '*' : ' ';
        push @text, sprintf( "%s%04d: %s", $highlight, $., $_ );
    }

    return join( '', @text, "\n" );
}

1;
