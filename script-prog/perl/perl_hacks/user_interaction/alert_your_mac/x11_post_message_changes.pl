unless ( $pid )
{
    sleep $growl_time;
    system( 'xmessage', @message );
}
