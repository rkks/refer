{
    package Tie::Ordinalize;

    use Lingua::EN::Numbers::Ordinate 'ordinate';
    use base 'Tie::Array';

    sub TIEARRAY  { return bless {}, shift } # dummy obj
    sub FETCH     { return ordinate( $_[1] ) }
    sub FETCHSIZE { return 0 }
}

tie my @TH, Tie::Ordinalize;
print $TH[4], "!\n";
