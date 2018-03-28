	#!/usr/bin/perl 
	#genproc.plx
	use warnings;
	use CGI::Pretty qw(:all);
	use strict;
	
	print header();
	if (param('first') and param('last')) {
	    my $first=ucfirst(lc(param('first')));
	    my $last=ucfirst(lc(param('last')));
	    print start_html("Welcome"),h1("Hello, $first $last");
	} else {
	    print start_html(title=>"Enter your name");
	    if (param('first') or param('last')) {
	         print center(font({color=>'red'},"You must enter a",
	         (param('last')?"first":"last"),"name"));
	    }
	    print generate_form();
	}
	print end_html();
	sub generate_form {
	    return start_form,
	    h1("Please enter your name:"),
	    p("Last name", textfield('last')),
	    p("First name", textfield('first')),
	         p(submit),
	         end_form;
	}

