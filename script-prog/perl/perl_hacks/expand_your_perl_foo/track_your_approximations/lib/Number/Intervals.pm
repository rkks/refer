package Number::Intervals;

# Compute maximal error in the representation of a given number...
sub _eps_for
{
    my ($num, $epsilon) = (shift) x 2;              # copy arg to both vars
    $epsilon /= 2 while $num + $epsilon/2 != $num;  # whittle epsilon down
    return $epsilon;
}

# Create an interval object, allowing for representation errors...
sub _interval
{
    use List::Util qw( min max );
    my ($min, $max) = ( min(@_), max(@_) );
    return bless [$min - _eps_for($min), $max + _eps_for($max)], __PACKAGE__;
}

# Convert all floating-point constants to interval objects...
sub import
{
    use overload;

    overload::constant(
        float => sub
        {
            my ($raw, $cooked) = @_;
            return _interval($cooked);
        },
    );
}

1;
