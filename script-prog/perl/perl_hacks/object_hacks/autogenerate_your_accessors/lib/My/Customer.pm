package My::Customer;

use strict;
use warnings;

sub new { bless {}, shift }

sub first_name
{
    my $self            = shift;
    return $self->{first_name} unless @_;
    $self->{first_name} = shift;
    return $self;
}

sub last_name
{
    my $self           = shift;
    return $self->{last_name} unless @_;
    $self->{last_name} = shift;
    return $self;
}

sub full_name
{
    my $self = shift;
    return join ' ', $self->first_name(), $self->last_name();
}

1;
