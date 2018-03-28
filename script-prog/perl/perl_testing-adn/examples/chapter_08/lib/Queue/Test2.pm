package Queue::Test;

use base 'Test::Class';

use Queue;
use Test::More;

sub setup_queues : Test( setup => 2 )
{
    my ($self) = @_;

    $self->{empty}    = Queue->new();
    $self->{twoitems} = Queue->new(qw( howdy bonjour ));

    isa_ok( $self->{$_}, 'Queue' ) for qw( empty twoitems );
}

sub size : Test(2)
{
    my ($self) = @_;
    is( $self->{empty}->size(),    0, 'an empty queue'             );
    is( $self->{twoitems}->size(), 2, 'a queue with some elements' );
}

sub enqueue : Test(1)
{
    my ($self) = @_;
    $self->{twoitems}->enqueue($_) for qw( ciao yo );
    is( $self->{twoitems}->size(), 4, 'queue is now larger' );
}

sub dequeue : Test(3)
{
    my ($self) = @_;

    is( $self->{empty}->dequeue(),    undef,     'empty queue' );

    is( $self->{twoitems}->dequeue(), 'howdy',   'first item'  );
    is( $self->{twoitems}->dequeue(), 'bonjour', 'second item' );
}

1;
