#!/usr/bin/perl

use strict;
use warnings;

use Net::Proxy;

# show some information on STDERR
Net::Proxy->set_verbosity(1);

# run this on the server that should listen on port 443
my $proxy = Net::Proxy->new(
    {   in =>
        {
            type         => 'dual',
            host         => '0.0.0.0',
            port         => 443,
            client_first =>
            {
                type => 'tcp',
                port => 444,     # move the https server to another port
            },
            server_first =>
            {
                type => 'tcp',
                port => 22,      # good old SSH
            },

            # wait during a 2 second timeout
            timeout      => 2,
        },
        out => { type => 'dummy' },
    }
);

$proxy->register();

Net::Proxy->mainloop();
