#!perl

BEGIN
{
        chdir 't' if -d 't';
}

use strict;
use warnings;
use Test::More tests => 6;

use Inline C =>
        Config   =>
                LIBS   => '-lm',
                ENABLE => 'AUTOWRAP'
;

Inline->import( C => <<END_HEADERS );
        double fmax( double, double );
        double fmin( double, double );
END_HEADERS

is( fmax(  1.0,  2.0 ),  2.0, 'fmax() should find maximum of two values' );
is( fmax( -1.0,  1.0 ),  1.0, '... and should handle one negative'       );
is( fmax( -1.0, -7.0 ), -1.0, '... or two negatives'                     );
is( fmin(  9.3,  1.7 ),  1.7, 'fmin() should find minimum of two values' );
is( fmin(  2.0, -1.0 ), -1.0, '... and should handle one negative'       );
is( fmin( -1.0, -6.0 ), -6.0, '... or two negatives'                     );
