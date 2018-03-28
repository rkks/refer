	#!/usr/bin/perl
	#starthtml.plx
	use warnings;
	use CGI qw(Link myheadertag);
	use strict;
	
	my $cgi=new CGI;
	
	print $cgi->header();
	print $cgi->start_html(
		-title => 'A complex HTML document header',
		-author=> 'sam.gangee@hobbiton.org',
		-xbase => 'http://www.theshire.net',
		-target => '_map_panel',
		-meta => 	{
					keywords => 'CGI header HTML',
					description => 'How to make a big header',
					message => 'Hello World!'
				},
		-style =>	{
					src => '/style/fourthage.css'
				},
		-head	=>	[
					Link({-rel=>'origin',
						-href=>'http://hobbiton.org/samg'}),
					myheadertag({-myattr=>'myvalue'}),
				]
		);
	print $cgi->end_html();
