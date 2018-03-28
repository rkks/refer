unless ( $pid )
{
    sleep $growl_time;
    system( qw( cmd net send localhost ), @message );
}
