package Queue::Test;

use base 'Test::Class';

use Queue;
use Test::More;

sub size : Test(4)
{
    my $q1 = Queue->new();
    isa_ok( $q1, 'Queue' );
    is( $q1->size(), 0, 'an empty queue' );

    my $q2 = Queue->new(qw( howdy bonjour ));
    isa_ok( $q2, 'Queue' );
    is( $q2->size(), 2, 'a queue with some elements' );
}

sub enqueue : Test(2)
{
    my $queue = Queue->new();
    isa_ok( $queue, 'Queue' );

    $queue->enqueue($_) for qw( howdy bonjour );
    is( $queue->size(), 2, 'queue is now larger' );
}

sub dequeue : Test(6)
{
    my $queue = Queue->new();
    isa_ok( $queue, 'Queue' );

    is( $queue->dequeue, undef, 'empty queue' );

    $queue->enqueue($_) for qw( howdy bonjour );
    is( $queue->size(),    2,         'queue is now larger'  );
    is( $queue->dequeue(), 'howdy',   'first item'           );
    is( $queue->dequeue(), 'bonjour', 'second item'          );
    is( $queue->size(),    0,         'queue is now smaller' );
}

1;
