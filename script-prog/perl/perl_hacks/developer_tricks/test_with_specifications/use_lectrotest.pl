# loads and activates LectroTest
use Test::LectroTest;

Property
{
    ##[ x <- Float ]##                  # first  part
    sqrt( $x * $x ) == $x;              # second part
}, name => "sqrt is inverse of square"; # third  part
