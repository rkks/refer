		#!/usr/bin/perl 
	#cookie2.plx
	use warnings;
	print "content-type: text/html\n\n";
	use CGI::Cookie;
	use strict;
	
	my $cookie2=new CGI::Cookie(-name=>"myCookie2",-value=>"fghij");
	print "Cookie 2: $cookie2\n";
	
