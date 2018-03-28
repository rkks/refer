	#!/usr/bin/perl 
	#pretty.plx
	use warnings;
	use strict;
	use CGI::Pretty qw(:standard);
	
	my $cgi=new CGI::Pretty;
	print header,
	      start_html("Pretty HTML Demo"),
	      ol(li(["First","Second","Third"])),
	      end_html;
