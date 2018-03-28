package My::Customer;

use strict;
use warnings;

use Class::MethodMaker
[
    new    => [qw( new )],
    scalar => [qw( first_name last_name )],
];

sub full_name
{
    my $self = shift;
    return join ' ', $self->first_name(), $self->last_name();
}
