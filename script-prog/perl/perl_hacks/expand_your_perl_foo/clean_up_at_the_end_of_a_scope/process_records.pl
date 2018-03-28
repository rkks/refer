use Scope::Guard;

sub process_records
{
    my $records  = fetch_records();
    my $last_rec = 0;
    my $cleanup  = sub { cleanup( $last_rec ) if $last_rec };
    my $guard    = Scope::Guard->new( $cleanup );

    for my $record ( @$records )
    {
        process_record( $record );
        $last_rec = $record;
    }
}

sub cleanup
{
    my $record = shift;

    # mark record as last record successfully completed
}
