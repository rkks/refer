	#!/usr/bin/perl 
	#starthtml_body.plx
	use warnings;
	use CGI::Pretty;
	use strict;
	
	my $cgi=new CGI;
	
	print $cgi->header();
	print $cgi->start_html(
		-title=>'A Red Background',
		-bgcolor=>'red'
		);
	print $cgi->h1("This page is red");
	print $cgi->end_html();
