# helper:  returns true if calling the given function
# results in an error; returns false otherwise

sub gives_error(&)
{
    ! eval { shift->() } and $@ ne "";
}

Property
{
    ##[ x <- Float ]##
    $x < 0 ? gives_error { sqrt( $x ) }
           : sqrt( $x * $x ) == $x
}, name => "sqrt is inverse of square and dies on negatives";
