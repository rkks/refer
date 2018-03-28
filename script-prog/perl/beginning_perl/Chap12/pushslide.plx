		#!/usr/bin/perl
	#pushslide.plx
	use warnings;
	use CGI::Push qw(:standard);
	use strict;
	
	do_push(-next_page=>\&show_slide,
	   -last_page=>\&go_back,
	   -type=>'dynamic',
	   -delay=>5
	);
	
	sub show_slide {
	   my ($cgi,$count)=@_;
	
	   # stop after 10 slides
	   return undef if $count>10;
	
	   #set content type in subroutine
	   my $slide=header();
	
	   # generate contents
	   $slide.=h1("This is slide $count");
	   return start_html("Slide $count").$slide.end_html();
	}
	sub go_back {
	   my $url=$ENV{'HTTP_REFERER'}; #try for the starting page
	   $url='/' unless defined $url; #otherwise default to the home page
	
	   #generate a 'refresh' header to redirect the client
	   return header(refresh=>"5; URL=$url", type=>"text/html"),
	   start_html("The End"),
	   p({-align=>"center"},"Thanks for watching!"),
	   end_html();
	}
