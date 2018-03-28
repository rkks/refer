		#!/usr/bin/perl 
	#calling.plx
	use warnings;
	use CGI;
	use strict;

print "Content-type: text/html\n\n";

	my $cgi=new CGI;
	print $cgi->start_html();
	
	print $cgi->center("Object method");
	print CGI->center("Class method");
	print CGI::center("Function call");
	
	print $cgi->end_html();
