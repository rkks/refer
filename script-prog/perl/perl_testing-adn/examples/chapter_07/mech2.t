#!perl

use WWW::Mechanize;

my $agent = WWW::Mechanize->new( autocheck => 1 );

$agent->get("http://search.cpan.org/");
$agent->field("query", "gerbil counting");
$agent->submit_form(form_name => "f");

$agent->follow_link(text => "Home", n => "1");
$agent->field("query", "test");
$agent->submit_form(form_name => "f");

$agent->follow_link(text => "Next >>", n => "1");
