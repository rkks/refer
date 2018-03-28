	#!/usr/bin/perl
	#forkedopen.plx
	use warnings;
	use strict;
	
	my $date;
	my $format="%s";
	
	unless (open DATE,"-|") {
		exec '/bin/date','-u',"+$format";
		#exec replaces our script so we never get here
	}
	$date=<DATE>;
	close DATE;
	print "Date 1:$date\n";
		
	my $result=open (DATE,"-|");
	exec '/bin/date','-u',"+$format" unless $result;
	$date=<DATE>;
	close DATE;
	print "Date 2:$date\n";
		
	open (DATE,"-|") || exec '/bin/date','-u',"+$format";
	$date=<DATE>;
	close DATE;
	print "Date 3:$date\n";
