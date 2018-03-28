package Queue;

use strict;
use warnings;

sub new
{
    my ($class, @items) = @_;
    bless \@items, $class;
}

sub size
{
    my ($self) = @_;
    return scalar @$self;
}

sub enqueue
{
    my ( $self, $item ) = @_;
    push @$self, $item;
}

sub dequeue
{
    my ( $self ) = @_;
    return shift @$self;
}

1;
