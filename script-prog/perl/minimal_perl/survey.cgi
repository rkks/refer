#! /usr/bin/perl
# survey.cgi: Demonstrates use of HTML forms with CGI

use strict;
use CGI qw(:standard);         # request CGI.pm components

# Carp shows errors in browser window, to facilitate debugging
use CGI::Carp qw(fatalsToBrowser);

# Store HTML code in variable $form; gets printed on screen, if
# run normally, or gets sent to browser, in CGI mode

if ( ! param() ) {     # if no parameters, we're displaying form
   my $form=header();           # prepare HTTP header for server
   $form.=start_html(-title=>'CGI Module Demo',
                     -BGCOLOR=>'ivory');
   $form.=h1('National Junk Food Survey');

   # action='' means this script will handle the submitted form
   $form.=startform(-method=>'GET', -action=>'');
   $form.=hr() . "\n";

   $form.=b('Please enter your first initial and surname:');
   $form.=p();
   $form.='Initial';
   $form.=textfield(-name=>'init', -size=>1, -maxlength=>1);
   $form.="\n";

   $form.='Surname';
   $form.=textfield(-name=>'name', -size=>12, -maxlength=>30);
   $form.="\n" . hr() . "\n";

   $form.=b('Please indicate your age group:');
   $form.=p();
   $form.=radio_group(-name=>'age',
                     -Values=>['Under 25', '25-40', 'Older']);
   $form.=hr() . "\n";

   $form.=b('What are your favorite ice-cream flavors?');
   $form.=p();
   $form.=checkbox_group(-name=>'flavor',
                  -Values=>['Chocolate', 'Vanilla', 'Other']);
   $form.=hr() . "\n";

   $form.=b('How many pizzas do you have monthly?');
   $form.="\n";
   $form.=popup_menu(-name=>'pizzas', -size=>2,
       -Values=>['no pizzas','one to three','four or more']);
   $form.=hr() . "\n";

   $form.=submit(-name=>'button', -value=>'SUBMIT');
   $form.="\n";
   $form.=endform();
   $form.=end_html();

   print $form;        # send HTML document to browser now
}

else {   # If parameters present, we're handling submitted form

   my $response=header();
   $response.=start_html(-title=>'Survey Response',
                         -BGCOLOR=>'snow');
   $response.=h1('National Junk Food Survey');
   $response.=p();
   $response.="You supplied these details:\n";
   $response.=Dump();
   $response.=p();
   $response.=h2( i('Thank you for participating!') );
   $response.=end_html();
   # Now send HTML string describing web page to browser
   print $response;
}
