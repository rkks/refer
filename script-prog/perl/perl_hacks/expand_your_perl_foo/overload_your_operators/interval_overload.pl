# Overload operators for Number::Intervals objects...
use overload
(
    # Add two intervals by independently adding minima and maxima...
    q{+} => sub
    {
        my ($x, $y) = _check_args(@_);
        return _interval($x->[0] + $y->[0], $x->[1] + $y->[1]);
    },

    # Subtract intervals by subtracting maxima from minima and vice versa...
    q{-} => sub
    {
        my ($x, $y) = _check_args(@_);
        return _interval($x->[0] - $y->[1], $x->[1] - $y->[0]);
    },

    # Multiply intervals by taking least and greatest products...
    q{*} => sub
    {
        my ($x, $y) = _check_args(@_);
        return _interval($x->[0] * $y->[0], $x->[1] * $y->[0],
                         $x->[1] * $y->[1], $x->[0] * $y->[1],
                        );
    },

    # Divide intervals by taking least and greatest quotients...
    q{/} => sub
    {
        my ($x, $y) = _check_args(@_);
        return _interval($x->[0] / $y->[0], $x->[1] / $y->[0],
                         $x->[1] / $y->[1], $x->[0] / $y->[1],
                        );
    },

    # Exponentiate intervals by taking least and greatest powers...
    q{**} => sub
    {
        my ($x, $y) = _check_args(@_);
        return _interval($x->[0] ** $y->[0], $x->[1] ** $y->[0],
                         $x->[1] ** $y->[1], $x->[0] ** $y->[1],
                        );
    },

    # Integer value of an interval is integer value of bounds...
    q{int} => sub
    {
        my ($x) = @_;
        return _interval(int $x->[0], int $x->[1]);
    },

    # Square root of interval is square roots of bounds...
    q{sqrt} => sub
    {
        my ($x) = @_;
        return _interval(sqrt $x->[0], sqrt $x->[1]);
    },

    # Unary minus: negate bounds and swap upper/lower:
    q{neg} => sub
    {
        my ($x) = @_;
        return _interval(-$x->[1], -$x->[0]);
    },

    # etc. etc. for the other arithmetic operators...
);
