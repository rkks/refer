		#!/usr/bin/perl
	#401namedresponse.plx
	use strict;
	use warnings;
	use CGI;
	my $cgi=new CGI;
	
	print $cgi->header(-type=>'text/html',
			-status=>'401 Authorization Required',
			-authname=>'Quo Vadis');
