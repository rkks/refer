Property
{
    ##[ x <- Float ]##
    $tcon->retry() if $x < 0;      # only test non-negatives
    sqrt( $x * $x ) == $x;
}, name => "sqrt is inverse of square";

Property
{
    ##[ x <- Float ]##
    $tcon->retry() unless $x < 0;  # only test negatives
    gives_error { sqrt( $x ) };
}, name => "sqrt of negative numbers gives error";
