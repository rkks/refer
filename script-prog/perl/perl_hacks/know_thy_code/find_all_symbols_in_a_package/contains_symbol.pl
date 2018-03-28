my %types =
(
    '$' => 'SCALAR',
    '@' => 'ARRAY',
    '%' => 'HASH',
    '*' => 'IO',
    '&' => 'CODE',
);

sub UNIVERSAL::contains_symbol
{
    my ($namespace, $symbol) = @_;
    my @keys                 = split( /::/, $namespace );
    my $type                 = $types{ substr( $symbol, 0, 1, '' ) }
                            || 'SCALAR';

    my $table = \%main::;

    for my $key (@keys)
    {
        $key .= '::';
        return 0 unless exists $table->{$key};
        $table = $table->{$key};
    }

    return 0 unless exists $table->{$symbol};
    return *{ $table->{$symbol} }{ $type } ? 1 : 0;
}
