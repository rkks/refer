	#!/usr/bin/perl 
	#table.plx
        use warnings;
	use CGI::Pretty;
	use strict;
	print "Content-type: text/html\n\n";
	
        my $cgi=new CGI;
		
	print $cgi->table({-border=>1,-cellspacing=>3,-cellpadding=>3},
			$cgi->Tr({-align=>'center',-valign=>'top'}, [
				$cgi->th(["Column1","Column2","Column3"]),
			]),
			$cgi->Tr({-align=>'center',-valign=>'middle'}, [
				$cgi->td(["Red","Blue","Yellow"]),
				$cgi->td(["Cyan","Orange","Magenta"]),
				$cgi->td({-colspan=>3},["A wide row"])
			]),
			$cgi->caption("An example table")
		);
