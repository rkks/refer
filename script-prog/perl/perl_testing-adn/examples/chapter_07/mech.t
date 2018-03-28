#!perl

use strict;
use warnings;

use Test::More tests => 6;
use Test::WWW::Mechanize;

my $mech = Test::WWW::Mechanize->new();

$mech->get_ok( 'http://search.cpan.org/' );

$mech->title_is( 'search.cpan.org: The CPAN Search Site' );

$mech->form_name( 'f' );
$mech->field( 'query', 'frobnicate' );
$mech->select( 'mode', 'module'     );
$mech->submit();

$mech->content_contains( 'No matches' );

$mech->back();

$mech->field( 'query', 'test' );
$mech->submit();

$mech->content_like( qr/ Results .+ Found /sx );
$mech->content_lacks( 'No matches' );
$mech->page_links_ok();
