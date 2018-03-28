use B::TerseSize;

sub report_largest_sub
{
    my $package                  = shift;
    my ($symbols, $count, $size) = B::TerseSize::package_size( $package );
    my ($largest)                =
        sort { $symbols->{$b}{size} <=> $symbols->{$a}{size} }
        grep { exists $symbols->{$_}{count} }
        keys %$symbols;

    print "Total size for $package is $size in $count ops.\n";
    print "Reporting $largest.\n";
    B::TerseSize::CV_walk( 'root', $package . '::' . $largest );
}
