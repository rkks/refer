package Proxy::AccessControl;

use strict;
use warnings;

use Attribute::Handlers;

my %perms;

sub UNIVERSAL::perms
{
    my ($package, $symbol, $referent, $attr, $data) = @_;
    my $method                                      = *{ $symbol }{NAME};

    for my $permission (split(/\s+/, $data))
    {
        push @{ $perms{ $package }{ $method } }, $permission;
    }
}

sub dispatch
{
    my ($user, $class, $method, @args) = @_;

    return unless $perms{ $class }{ $method } and $class->can( $method );

    for my $perm (@{ $perms{ $class }{ $method } })
    {
        die "Need permission '$perm\n'" unless $user->has_permission( $perm );
    }

    $class->$method( @args );
}

1;
