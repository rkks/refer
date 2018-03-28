#!perl

use strict;
use warnings;

use Apache::Test;
use Apache::TestUtil;
use Apache::TestRequest qw( GET_BODY );
use Imager;

plan( tests => 1, need_module('mod_perl') );

my $content = GET_BODY('/images/panorama.jpg');

my $img     = Imager->new();
$img->open( data => $content, type => 'jpeg' )
    or die $img->errstr();

my $max     = 150;

t_debug( "assuming constraint is $max pixels" );

t_debug( 'width: '  . $img->getwidth()    );
t_debug( 'height: ' . $img->getheight()   );

ok( ( $img->getwidth() == $max ) or ( $img->getheight() == $max ) );
