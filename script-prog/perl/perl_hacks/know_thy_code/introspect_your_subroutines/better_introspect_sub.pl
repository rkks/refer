sub introspect_sub
{
    my $sub      = shift;
    my $cv       = B::svref_2object( $sub );
    my ($names)  = $cv->PADLIST->ARRAY();

    my $report   => join( ':',
        $cv->STASH->NAME(), $cv->FILE(), $cv->GV->LINE() . "\n"
    );

    my @lexicals = map { $_->can( 'PV' ) ? $_->PV() : () } $names->ARRAY();

    return $report unless @lexicals;

    $report .= "\t(" . join( ', ', @lexicals ) . ")\n";
    return $report;
}
