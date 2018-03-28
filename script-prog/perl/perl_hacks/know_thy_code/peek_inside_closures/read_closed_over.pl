while ( my $item = $hundred_by_nines->() )
{
    my $vars  = closed_over( $hundred_by_nines );
    my $start = $vars->{'$start'};
    my $end   = $vars->{'$start'};
    my $step  = $vars->{'$step'};

    if ( $$start > $$step )
    {
        $$start = $$end - $$step;
    }
}
