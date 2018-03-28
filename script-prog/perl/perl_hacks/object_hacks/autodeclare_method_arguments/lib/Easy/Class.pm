package Easy::Class;

use strict;
use warnings;

use Attribute::Method qw( $status );

sub new :Method
{
    bless { @_ }, $self;
}

sub set_status :Method( $status )
{
    $self->{status} = $status;
}

sub get_status :Method
{
    return $self->{status};
}

1;
