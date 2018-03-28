package My::Customer;

use strict;
use warnings;

use Class::BuildMethods qw(
  first_name
  last_name
);

# Note that you can use an array reference, if you prefer
sub new { bless [], shift }

sub full_name
{
    my $self = shift;
    return join ' ', $self->first_name(), $self->last_name();
}

1;
