my @NUMERAL_FOR    = (0..9,'A'..'Z');

sub convert_to_base($base, $number)
{
    my $converted  = "";
    while ($number > 0)
    {
        $converted = $NUMERAL_FOR[$number % $base] . $converted;
        $number    = int( $number / $base);
    }
    return $converted;
}
