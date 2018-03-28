		#!/usr/bin/perl
	#envdump.plx
	use warnings; 
	use strict;
	use CGI::Pretty;
	
	my $cgi=new CGI::Pretty;
	
	print	$cgi->header(),	
		$cgi->start_html("Environment Dumper"),
		$cgi->table({-border=>1},
				$cgi->Tr($cgi->th(["Parameter","Value"])),
				map {
					$cgi->Tr($cgi->td([$_,$ENV{$_}]))
				} sort keys %ENV
			),
		$cgi->end_html();
