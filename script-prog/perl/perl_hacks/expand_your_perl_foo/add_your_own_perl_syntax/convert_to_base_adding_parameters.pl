sub convert_to_base { my ($base, $number) = @_;
    my $converted  = '';
    while ($number > 0)
    {
        $converted = $NUMERAL_FOR[$number % $base] . $converted;
        $number    = int( $number / $base);
    }
    return $converted;
}
