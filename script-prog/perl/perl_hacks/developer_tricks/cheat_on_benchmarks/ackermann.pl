use strict;
use warnings;

no warnings 'recursion';

sub ackermann
{
    my ($m, $n) = @_;
    return            $n + 1      if $m == 0;
    return ackermann( $m - 1, 1 ) if $n == 0;
    return ackermann( $m - 1, ackermann( $m, $n - 1 ) );
}

print ackermann( 3, 10 ), "\n";
