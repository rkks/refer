		#!/usr/bin/perl 
	#email_secure.plx
	use warnings;
	use strict;
	
	#use CGI 
	use CGI qw(:all);
	$CGI::POST_MAX=100; #limit size of POST
	
	#set the search path explicitly
	$ENV{'PATH'}="/bin";
	
	print header(),start_html("Mail yourself a greeting");
	my $mail_to=param('email');
		
	#check the email address is decent
	if (not $mail_to or $mail_to !~ /\@/) {
		print start_form,
		      h2("Please enter an email address"),
			p(textfield('email')),
			p(submit),
			endform;
	} elsif ($mail_to =~ tr/;<>*¦`&$!#[]{}:'"//) {
		print h2("Invalid address");
	} else {
		#run an external mail program
		if (open MAIL,"¦mail $mail_to") {
			print MAIL "Hello from Email!\n";
			close MAIL;
			print h1("Greeting Sent!");
		} else {
			print h2("Failed to send: $!");
		}
	}
	print end_html();


