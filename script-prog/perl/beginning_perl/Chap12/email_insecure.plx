		#!/usr/bin/perl
	#email_insecure.plx
	use warnings;
	print "Content-Type: text/html\n\n";
	
	$mail_to=$ENV{'QUERY_STRING'};
	print "<HTML><HEAD><TITLE>Mail yourself a greeting</TITLE>";
	print "</HEAD><BODY><H1>Greeting Sent!</H1>";
	print "</BODY></HTML>";
	
	open (MAIL,"|mail dlmercer@hotmail.com"); #run an external mail program
	print MAIL "Hello from Email!\n";
	close MAIL;


