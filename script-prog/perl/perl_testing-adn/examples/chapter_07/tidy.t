#!perl

use strict;

use Test::More tests => 2;
use Test::WWW::Mechanize;
use Test::HTML::Tidy;

my $mech = Test::WWW::Mechanize->new();

$mech->get_ok( 'http://search.cpan.org/' );

html_tidy_ok( $mech->content );

$mech->field( 'query', 'otter spotting' );
$mech->submit();

html_tidy_ok( $mech->content() );
