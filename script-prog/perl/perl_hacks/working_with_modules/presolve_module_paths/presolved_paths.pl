BEGIN
{
    require( $INC{'CGI.pm'}      = '/usr/lib/perl5/5.8.7/CGI.pm' );
    require( $INC{'CGI/Util.pm'} = '/usr/lib/perl5/5.8.7/CGI/Util.pm' );
    require( $INC{'vars.pm'}     = '/usr/lib/perl5/5.8.7/vars.pm' );
    require( $INC{'constant.pm'} = '/usr/lib/perl5/5.8.7/constant.pm' );
    require( $INC{'overload.pm'} = '/usr/lib/perl5/5.8.7/overload.pm' );
}

1;
