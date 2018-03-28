#!/usr/bin/perl 
#CGIpara.plx
use strict;
use warnings;

	use CGI;
	
	my $cgi=new CGI; #read in parameters
	
	#iterate over each parameter name
	foreach ($cgi->param()) {
	    #modify and set each parameter value from itself
	    $cgi->param($_,ucfirst(lc($cgi->param($_))));
	}
        
        print $cgi->header(); #print a header
	print $cgi->start_html("Welcome");      #generate HTML document start
	print "<h1>Welcome, ",$cgi->param('first')," ",$cgi->param('last'),"</h1>";
        print $cgi->end_html(); 
