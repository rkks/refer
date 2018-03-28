use B;

sub introspect_sub
{
    my $sub      = shift;
    my $cv       = B::svref_2object( $sub );

    return join( ':',
        $cv->STASH->NAME(), $cv->FILE(), $cv->GV->LINE() . "\n"
    );
}
