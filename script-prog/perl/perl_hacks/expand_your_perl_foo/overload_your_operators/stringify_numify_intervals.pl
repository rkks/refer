use overload
(
    # Stringify intervals as: VALUE (E<plusmn>UNCERTAINTY)...
    q{""} => sub
    {
        my ($self) = @_;

        my $uncert = ($self->[1] - $self->[0]) / 2;

        use charnames qw( :full );
        return $self->[0]+$uncert . " (\N{PLUS-MINUS SIGN}$uncert)";
    },

    # Numerify intervals by averaging their bounds (with warning)...
    q{0+} => sub
    {
        my ($self) = @_;
        carp "Approximating interval by a single (averaged) number";
        return ($self->[0] + $self->[1]) /2;
    },
);
