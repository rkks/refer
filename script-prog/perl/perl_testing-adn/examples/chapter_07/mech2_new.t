#!perl

use strict;

use Test::More tests => 3;
use Test::WWW::Mechanize;

my $agent = Test::WWW::Mechanize->new;

$agent->get_ok( 'http://search.cpan.org/' );
$agent->field( 'query', 'gerbil counting' );
$agent->submit_form( form_name => 'f' );

$agent->follow_link_ok( { text => 'Home', n => '1' } );
$agent->field( 'query', 'test' );
$agent->submit_form( form_name => 'f' );

$agent->follow_link_ok( { text => 'Next >>', n => '1' } );
