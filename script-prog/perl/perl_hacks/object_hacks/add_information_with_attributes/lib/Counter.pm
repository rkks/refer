package Counter;

use strict;
use warnings;

use Attribute::Docstring;

our $counter :Doc( 'a count of all new Foo objects' );

sub new :Doc( 'the constructor for Foo' )
{
    $counter++;
    bless {}, shift;
}

sub get_count :Doc( 'returns the count for all foo objects' )
{
    return $counter;
}

1;
