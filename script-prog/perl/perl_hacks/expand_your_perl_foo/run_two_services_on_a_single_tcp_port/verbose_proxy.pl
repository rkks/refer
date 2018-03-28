#!/usr/bin/perl

use strict;
use warnings;

use Net::Proxy;

# show some information on STDERR
Net::Proxy->set_verbosity(1);

# run this on your workstation
my $proxy = Net::Proxy->new(
    {   in =>
        {
            # local port for local SSH client
            port => 2222,
            type => 'tcp',
        },
        out =>
        {
            host        => 'home.example.com',
            port        => 443,
            proxy_host  => 'proxy.company.com',
            proxy_port  => 8080,
            proxy_user  => 'id23494',
            proxy_pass  => 's3kr3t',
            proxy_agent => 'Mozilla/4.0 (compatible; MSIE 6.0; Windows XP)',
        },
    }
);

$proxy->register();

Net::Proxy->mainloop();
