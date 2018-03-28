		#!/usr/bin/perl 
	#session.plx
	use warnings;
	use Apache::Session::File;
	use CGI;
	use CGI::Carp;
	
	my $cgi=new CGI;
	my $cookie=$cgi->cookie("myCookie"); # existing cookie or undef
	
	eval {
		# $cookie is existing cookie or undef to create a new session
		tie %session, 'Apache::Session::File', $cookie,
			{Directory => '/tmp/sessions/'};
	};
	
	if ($@) {
		if ($@=~/^Object does not exist in the data store/) {
			# session does not exist in file (expired?) - create a new one
			tie %session,'Apache::Session::File', undef,
				{Directory => '/tmp/sessions/'};
			$cookie=undef; # this cookie isn't valid any more, undef it. 
		} else {
# some other more serious error has occurred and session
			# management is not working.
			print $cgi->header('text/html','503 Service Unavailable');
			die "Error: $@ ($!)";
			exit;
		}
	}
	
	unless ($cookie) {
		# retrieve the new session id from the %session hash
		$cookie=$cgi->cookie(-name=>"myCookie",
				     -value=>$session{_session_id},
				     -expires=>"+1d");
		print $cgi->header(-type=>"text/html",-cookie=>$cookie);
	} else {
		print $cgi->header(); #no need to send cookie again
	}
	
	print $cgi->start_html("Session Demo"),
		$cgi->h1("Hello, you are session id ",$session{_session_id}),
		$cgi->end_html;
	
	untie %session;

