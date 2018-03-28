use strict;
use warnings;

use HTTP::Proxy ':log';
use HTTP::Proxy::HeaderFilter::simple;

# start the proxy with the given command-line parameters
my $proxy = HTTP::Proxy->new( @ARGV );

for my $redirect (<DATA>)
{
    chomp $redirect;

    my ($pattern, $destination) = split( /\|/, $redirect );
    my $filter                  = get_filter( $destination );

    $proxy->push_filter( host => $pattern, request => $filter );
}

$proxy->start();

my %filters;

sub get_filter
{
    my $site = shift;

    return $filters{ $site } ||= HTTP::Proxy::HeaderFilter::simple->new(
        sub
        {
            my ( $self, $headers, $message ) = @_;

            # modify the host part of the request only
            $message->uri()->host( $site );

            # create a new redirect response
            my $res = HTTP::Response->new(
                301,
                "Moved to $site", 
                [ Location => $message->uri() ]
            );

            # and make the proxy send it back to the client
            $self->proxy()->response( $res );
        }
    );
}

__DATA__
perlmonks.com|perlmonks.org
www.perlmonks.org|perlmonks.org
