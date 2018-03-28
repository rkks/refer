#!/usr/bin/perl 
use warnings;
use CGI::Pretty qw(:all);
use strict;

my $cgi=new CGI;

print header();
if ($cgi->param('first') and $cgi->param('last')) {
	my $first=ucfirst(lc($cgi->param('first')));
	my $last=ucfirst(lc($cgi->param('last')));
	print start_html("Welcome"),h1("Hello, $first $last");
} else {
	print start_html(-title=>"Enter your name");
	if ($cgi->param('first') or $cgi->param('last')) {
		print center(font({-color=>'red'},"You must enter a",
			($cgi->param('last')?"first":"last"),"name"));
	}
	print generate_form();
}
print end_html();

sub generate_form {
	return start_form,
		h1("Please enter your name:"),
		p("First name", textfield('first')),
		p("Last name", textfield('last')),
		p(submit),
		end_form;
}