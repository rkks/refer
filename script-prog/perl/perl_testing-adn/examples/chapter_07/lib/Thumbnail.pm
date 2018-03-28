package Thumbnail;

use strict;
use warnings;

use Apache::Constants qw(:common);
use Apache::File;
use Imager;

our $constraint = 150;

sub handler
{
    my ($r) = @_;

    return DECLINED unless $r->content_type() =~ m{^image/};

    my $img = Imager->new();
    $img->open( file => $r->filename ) or die $img->errstr();

    $img    = $img->scale( xpixels => $constraint, ypixels => $constraint );

    my ( $tmpfile, $tmpfh ) = Apache::File->tmpfile();
    $img->write( file => $tmpfile, type => 'jpeg' )
        or die $img->errstr();

    $r->send_http_header('image/jpeg');
    $r->send_fd($tmpfh);

    return OK;
}

1;
