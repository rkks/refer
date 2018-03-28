# Flip args if necessary, converting to an interval if not already...
sub _check_args
{
    my ($x, $y, $reversed) = @_;

    return $reversed              ?  ( _interval($y), $x            )
         : ref $y ne __PACKAGE__  ?  ( $x,            _interval($y) )
         :                           ( $x,            $y            );
}
