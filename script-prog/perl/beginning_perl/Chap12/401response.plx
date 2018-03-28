		#!/usr/bin/perl
	#401response.plx
	use warnings;
	use strict;
	use CGI;
	
	my $cgi=new CGI;
	print $cgi->header('text/html','401 Authorization Required');

