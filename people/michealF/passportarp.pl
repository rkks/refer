#!/usr/bin/perl
#
# Filename: /var/www/cgi-bin/passportarp.pl
#
# Purpose:  Query Nortel Ethernet Routing Switch 8600 for IP ARP table.
#           Locate user provided IP address and identify switch card and
#           port location.
#
# Author:   Michael McNamara
#
# Date:     December 5, 2002
#
# Changes:
#
#    Sep 10, 2007 (M.McNamara)
#       o  added DBI code to build HTML form from database source
#    Feb 28, 2006 (M.McNamara)
#       o  added logic to handle SMLT links
#    May 11, 2004 (M.McNamara)
#       o  add the MAC address to the report output
#    Jul 11, 2003 (M.McNamara)
#       o  add additional input validatation
#
#
# Program Flow: 
#	Check CGI Variables
#	Query switch for general information
#	Output general information
#	Query switch for VLAN information
#	Query switch for MLT information
#	Loop over VLANs to retreive FDB for each VLAN
#	Search for MAC address while retreiving FDB
#	Output and hits as the search progresses
#	End
#
# Requirements:
# 	Net-SNMP Perl Library
# 	CGI Perl module
# 	DBI Perl module
#
# Notes: 
#   	This is a very rough Perl script designed just for the basics. 
#   	It is a great example of what's possible and given alittle time
#   	and effort, and perhaps some decent HTML style sheets, you could
#   	really build a very nice front end to managing anything with a
#   	decent SNMP mib behind it.
#
# 	I've only recently just added some DBI code to help automate
# 	the process of adding/removing switches from the network. You
# 	can comment out the DBI routines or better yet just provide your 
# 	switch FQDN (or IP address) on the URL parameter like so;
#
# 	http://127.0.0.1/cgi-bin/passportarp.pl?switch=ers8600.acme.or&ip=192.168.1.1

# Load Modules
use strict;
use SNMP;
use CGI;
use DBI;

# MySQL Database Paramaters
my $dbuser = "perlscript";
my $dbpasswd = "specialpass";
my $database = "technology";
my $table = "devices";
my $dsn="DBI:mysql:technology:localhost";

# Declare constants
#use constant DEBUG      => 0;           # DEBUG settings
use constant RETRIES    => 3;           # SNMP retries
use constant TIMEOUT    => 1000000;     # SNMP timeout, in microseconds
use constant SNMPVER    => 1;           # SNMP version

# Declaration Variables
my ($sess, @vals);
my @devices;
my ($card, $port);
my $snmphost;
my $comm = "5ninety";
my %array;
my $switchfile = "";
my $datafile = "";
my $tempcard;
my $tempport;

my $intf;
my ($sysDescr, $sysObjectID, $sysUpTime, $sysContact, $sysName, $sysLocation);
my $numvlans;
my $i;
my (@vlanidtable, @vlannametable);
my $tempf;
my $date;
my $ip;
my $mlt;
my $nummlts;
my $x;
my $specialcomm;
my @mltname;
my @mltindex;
my @mltifindex;
my @vlanidx;
my $found = 0;
my $count = 0;
my ($days, $hrs, $mins, $tTime);

my $DEBUG = 0;		# DEBUG FLAG for troubleshooting

# SNMP Settings
$SNMP::verbose = $DEBUG;
$SNMP::use_enums = 1;
$SNMP::use_sprint_value = 0;
&SNMP::initMib();
#&SNMP::loadModules('ALL');
&SNMP::loadModules('RAPID-CITY');
### NOTE ABOVE THAT I HAD ISSUES WITH SOME OF THE OTHER SNMP MIBS, SO I
### CHOSE JUST TO LOAD JUST THE RAPID-CITY (ERS8600) SNMP-MIB BY ITSELF

undef @devices;

# Lets disable IO buffering in case we've enabled debugging
$| = 1;

# Lets create a new CGI object
my $web = new CGI;

# Lets read the variables being passed by CGI
$snmphost = $web->param('switch');
$ip = $web->param('ip');
$DEBUG = $web->param('debug');

# In the real world we should definitely check the three input strings
# above to make sure that they are not tainted or clean up and formatting
# issues we might have with the input. 
$ip =~ tr/a-z/A-Z/;		# Converting MAC address to uppercase

#########################################################################
#########################################################################
##  M  A  I  N     P  R  O  G  R  A  M
#########################################################################
#########################################################################

&html_header;		# HTML Header

# If there's no $snmphost name supplied lets build an HTML form
if ($snmphost eq "") {
   &html_form();
} else {
   &runjob();
}

&html_footer;		# HTML Footer

exit 0;

#########################################################################
#########################################################################
###   E  N  D    P  R  O  G  R  A  M
#########################################################################
#########################################################################

###########################################################################
# Subroutine html_header 
#
# Purpose: perform all the initialization steps and procedures
###########################################################################

sub html_header{

   print $web->header;
   print $web->start_html('Nortel Passport 8600 Gigabit Switch IP ARP Table Search');
   print "<FONT Face=Tahoma>";
   print $web->h3('Nortel Passport 8600 Gigabit Switch IP ARP Table Search');

} #end sub

###########################################################################
# Subroutine html_footer
#
# Purpose: perform all the initialization steps and procedures
###########################################################################
sub html_footer
{
   print $web->end_html;

} #end sub


###########################################################################
# Subroutine html_form
#
# Purpose: perform all the initialization steps and procedures
###########################################################################
sub html_form
{

   # Declare Local Variables
   my $ers8610id="rcA8610";
   my $ers8606id="rcA8606";
   my $ers1648id="rcA1648";
   my $table="devices";
   my $method="POST";
   my $action="http://madmax.acme.org/cgi-bin/passportarp.pl";
   my $encoding="application/x-www-form-urlencoded";
   my $dbvalue;

   # Let's open a DBI connection to the MySQL database
   my $dbh = DBI->connect($dsn,$dbuser,$dbpasswd) or
      die "ERROR: cannot open database: $DBI::errstr\n";

   # Let's truncate the test table
   #my $truncate_sth = $dbh->prepare("TRUNCATE $table");
   #$truncate_sth->execute();
   #$truncate_sth->finish();

   my $query_sth = $dbh->prepare("SELECT DISTINCT hostname FROM $table WHERE (sysObjectID = '$ers8610id') OR (sysObjectID = '$ers8606id') OR (sysObjectID = '$ers1648id')");
   #my $query_sth = $dbh->prepare("SELECT hostname FROM $table");

   $query_sth->execute();

   #print $web->startform($method,$action,$encoding)

   print <<EOF;
<form method="POST" enctype="application/x-www-form-urlencoded" action="http://madmax.mdc.acme.org/cgi-bin/passportarp.pl">
  <p>
  <font face="Tahoma">Passport 8600 Switch</font>&nbsp;&nbsp;&nbsp;
  <select size="1" name="switch">
  <option selected>Select Passport Switch</option>
EOF


      while ($dbvalue = $query_sth->fetchrow_array()) {
            print "<option>$dbvalue</option>\n";
      }



print <<EOF;
  </select>&nbsp;&nbsp;&nbsp; <font face="Tahoma" size="2" color="#FF0000">&lt;&lt;&lt; 
  Select a switch</font></p>
  <p><font face="Tahoma">IP Address&nbsp;&nbsp;
  <input type="text" name="ip" size="15" maxlength="15"> 
  <font size="2"> <i>
  (xxx.xxx.xxx.xxx)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <input type="checkbox" name="debug" value="no"> </i><font color="#0000FF">
	DEBUG</font></font></font></p>
  <p><font face="Tahoma"><input type="submit" value="Query" name="B1"><input type="reset" value="Reset" name="B2"></font></p>
</form>
Passport 8600 switches.</font></p>
<p><font face="Tahoma">Let me know if you have questions, problems or 
suggestions.</font></p>
<p><font face="Tahoma"><a href="mailto:">Mike</a></font></p>
<hr size="1">
EOF

   $query_sth->finish();

   $dbh->disconnect || die "Failed to disconnect\n";

} #end sub

###########################################################################
# Subroutine runjob 
#
# Purpose: perform all the initialization steps and procedures
###########################################################################
sub runjob {

   print "<BR><FONT SIZE=2>Initializing query for <I><B>$snmphost</B></I> for IP address <I><B>$ip</B></I>...<BR>\n";
   print "<P>\n";

   if ($DEBUG) {
      print "DEBUG: snmphost = $snmphost <BR>\n";
      print "DEBUG: ipaddr   = $ip       <BR>\n";
   }

   if (($snmphost eq "") || ($snmphost eq "Select Passport Switch")) {
      print "<B>ERROR: The switch parameter appears to be emtpy?<BR>\n";
      print "Please go back and enter a valid switch IP address of host name</B><BR>\n";
      exit 1;
   } elsif ($ip eq "") {
      print "<B>ERROR: The ip address parameter appears to be emtpy?<BR>\n";
      print "Please go back and enter a valid IP address</B><BR>\n";
      exit 1;
   } elsif ( (length($ip) < 7) | (length($ip) > 15) ) {
      print "<B>ERROR: The ip address shoud be greater than 6 characters and\n";
      print "less than 15 characters long!?!<BR>\n";
      print "Please go back and enter a valid IP address</B><BR>\n";
      exit 1;
   };

   # Lets create a new SNMP session to the switch
   $sess = new SNMP::Session (	DestHost   =>  $snmphost, 
				Community  =>  $comm,
				Retry      =>  RETRIES,
				Timeout    =>  TIMEOUT,
				Version    =>  SNMPVER );
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }


   my $vars = new SNMP::VarList(
				['sysDescr', 0],
				['sysObjectID', 0],
				['sysUpTime', 0],
				['sysContact', 0],
				['sysName', 0],
				['sysLocation', 0] );
  
   @vals = $sess->get($vars);	# retreive SNMP information
      if ( $sess->{ErrorStr} ) {
         print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
      }

   if ($vals[0] eq "") {
      print "<FONT COLOR=#FF0000><B>SNMP ERROR: Unable to poll the switch!!!<BR></FONT>\n";
      print "It is possible that the switch is down or you have <BR>\n";
      print "the wrong IP address or host name.<P>\n";
      print "Please go back and check the switch name.</B><BR>\n";

      print "DEBUG: vals[0] = $vals[0]<BR>\n";
      print "DEBUG: vals[1] = $vals[1]<BR>\n";
      print "DEBUG: vals[2] = $vals[2]<BR>\n";
      print "DEBUG: vals[3] = $vals[3]<BR>\n";
      print "DEBUG: vals[4] = $vals[4]<BR>\n";
      print "DEBUG: vals[5] = $vals[5]<BR>\n";
      print "DEBUG: vals[6] = $vals[6]<BR>\n";
      exit 1;
   }

   $sysDescr = $vals[0];
   $sysObjectID = $vals[1];
   $sysUpTime = $vals[2];
   $sysContact = $vals[3];
   $sysName = $vals[4];
   $sysLocation = $vals[5];

   $tTime = $sysUpTime;
   $tTime /= 100;
   $days = int($tTime/(60*60*24));
   $tTime -= $days * (60*60*24);
   $hrs = int($tTime/(60*60));
   $tTime -= $hrs*(60*60);
   $mins = int($tTime/(60));
   $tTime -= $mins*(60);

   print "sysDescr   = <B>$sysDescr</B><BR>\n";
   print "sysObjectID = <B>$sysObjectID</B><BR>\n";
   print "sysUpTime = <B>$days Days $hrs Hours $mins mins $tTime secs</B><BR>\n";
   print "sysContact = <B>$sysContact</B><BR>\n";
   print "sysName = <B>$sysName</B><BR>\n";
   print "sysLocation = <B>$sysLocation</B><BR>\n";

   $tempf = ((9/5 * $vals[6]) + 32);
 
   if ($tempf < 100) {
      print "Chassis Temperature = <B>", ((9/5 * $vals[6]) + 32), " degrees</B><BR>\n";
   } else {
   print "Chassis Temperature = <FONT COLOR=#FF0000><B>", ((9/5 * $vals[6]) + 32), " degrees</FONT></B><BR>\n";
   };
   print "<P>\n";
   print "<I><B>Please be patient it may take a while to complete the search...</B></I>\n";
   print "<P>\n";

################ BEGIN POLLING SPECIFIC SNMP PARAMETERS ######################

   # rcMltNumMlts
   $nummlts = $sess->get("rcMltNumMlts.0");
   print "DEBUG: Number of MultiLink Trunks = $nummlts<BR>\n" if ($DEBUG);

   for ($i = 1; $i <= $nummlts; $i++) {
      # rcMltName
      $mltname[$i] = $sess->get("rcMltName.$i");
      # rcMltId
      $mltindex[$i] = $sess->get("rcMltId.$i");
      # rcMltIfIndex
      $mltifindex[$i] = $sess->get("rcMltIfIndex.$i");
      print "DEBUG: MltId = $i and MltName = $mltname[$i] and MltIndex = $mltindex[$i] and MltIfIndex = $mltifindex[$i]<BR>\n" if ($DEBUG);
   };

   # rcVlanNumVlans
   $numvlans = $sess->get("rcVlanNumVlans.0");
   $numvlans = $numvlans - 1;
   print "<BR>DEBUG: Number of VLANs = $numvlans<BR>\n" if ($DEBUG);

   my $vars = new SNMP::VarList(
                  ['rcVlanId', 0],
                  ['rcVlanName', 0] );

   for ($i = 1; $i <= $numvlans; $i++) {

      ($vlanidtable[$i], $vlannametable[$i])= $sess->getnext($vars);
      print "DEBUG: VLANID = $vlanidtable[$i] with VLAN Name = $vlannametable[$i]<BR>\n" if ($DEBUG);

   } #end for loop numvlans

#sleep(10);

   # In the past you needed to construct a special SNMP string, appending 
   # the VLAN ID to the SNMP string in order to search the different FDB tables
   # thankfully this is no longer the case as of v3.7 software
   #for ($x = 1; $x <= ($numvlans); $x++) {
   #   $specialcomm = "$comm\@$vlanidtable[$x]";
   #   print "<BR>DEBUG: Special Community String = $specialcomm<BR>\n" if ($DEBUG);

   # Lets create a new SNMP session to the switch
   $sess = new SNMP::Session (  DestHost   =>  $snmphost,
                                Community  =>  $comm,
                                Retry      =>  RETRIES,
                                Timeoute   =>  TIMEOUT,
                                Version    =>  SNMPVER );

   # We'll using the SNMP MIB2 ipNetToMedia table. In the past we had used the 
   # vendor specific rcBridge table but by using this table we can adopt this
   # script to other devices/vendors/models/etc.
   my $vars = new SNMP::VarList(
                                ['ipNetToMediaIfIndex', 0],
                                ['ipNetToMediaPhysAddress', 0],
                                ['ipNetToMediaNetAddress', 0],
                                ['ipNetToMediaType', 0] );

   while (1) {

      # Lets perform the SNMP getnext
      @vals = $sess->getnext($vars);  # retreive SNMP information

      # Lets test to make sure the getnext has not walked outside of the MIB
     last unless ($vars->[0]->tag eq 'ipNetToMediaIfIndex');

      #print "SNMP query against $snmphost just completed\n" if ($DEBUG);

      # We need to unpack the MAC address into a human form
      $vals[1] = unpack('H12', $vals[1]);
      $vals[1] =~ tr/a-z/A-Z/;

      # This is the formula (thanks to Shane at Nortel Networks) we
      # can use to get the card value and port value from the IfIndex
      $tempcard = (($vals[0] & 62914560) / 4194304);
      $tempport = (($vals[0] & 4128768) / 65536) + 1;

      print "DEBUG: $snmphost; $vals[0] ($tempcard/$tempport); $vals[1]; $vals[2]; $vals[3]<BR>\n" if ($DEBUG);

      # Lets count the number of ARP entries we're seaching through
      $count++;

      # Lets test to see if our input IP address matches this record
      if ($ip eq $vals[2]) {

         $found = 1;				# SET FLAG

         $card = (($vals[0] & 62914560) / 4194304);
         $port = (($vals[0] & 4128768) / 65536) + 1;

         print "<BR>\n";
	 print "<B>DEVICE FOUND</B><BR>\n";

	 # In the past we would have been happy to just immediately report
	 # the card and port where we found the IP address in the ARP table.
	 # However, these days we need to be mindful of MLT and SMLT links
         #print "Card = <B>$card</B>,  Port = <B>$port</B><BR>\n";
	 #print "VLAN = <B>$vals[3]</B><BR>\n";	
 	 print "<BR>\n";

         if ($card != 0) {
            #$card = int($vals[2]/64);
            #$port = ($vals[2] - ($card * 64) + 1);
	    $intf = (((64 * $card) + $port) - 1);
	    print "<I><B>$ip</B> ($vals[1])</I> address found on card <B>$card</B> port <B>$port</B><BR>\n";
	 } else {
            $mlt = 1;
	    print "<I><B>$ip</B> ($vals[1])</I> address found on MLT <B>$mltname[$port]</B><BR>\n";
         } # end else
      } # end if ip=val
   } #end while

   # In the past I used the loop below to control the SNMP getnext loop,
   # eventually I found a better method as I'm currently using above.
   #do{
   #} until ($sess->{ErrorStr} | (($vals[3] ne "other")
   #                     & ($vals[3] ne "invalid")
   #                     & ($vals[3] ne "dynamic")
   #                     & ($vals[3] ne "static") ) );

   if ($found == 0) { 
      print "<B>We were unable to find that MAC address in any of the ";
      print "VLAN forwarding tables.<BR> The device may be shutoff or it ";
      print "may have aged out of the forwarding tables.<BR>\n";
      print "You may contact the Technology Infrastructure Team for ";
      print "further assistance.</B>\n";
   }

   if (($found) & !($mlt)) {
      print "<BR>\n";
      print "Would you like to check the status of this port? ";
      print "<A HREF=/cgi-bin/genericport.pl?switch=$snmphost&card=$card&port=$port&debug=$DEBUG>Click Here!!!</A><BR>\n";
      print "<P>\n"; 
      print "Would you like to check the utilization of this port? ";
      print "<A HREF=/cgi-bin/14all.cgi?log=$snmphost"."_"."$intf&cfg=$snmphost.cfg>Click Here!!!</A><BR>\n"
   }

   print "<P><B>We searched through $count forwarding records...</B><P>\n";

   # HTML formatting
   print "<P>\n";
   $date = `date`;
   print "<P>\n";
   print "<I>$date</I>\n";
   print "<P>\n";
   print "<IMG SRC=http://madmax.acme.org/images/powered_by_advx.png>";
   print "<IMG SRC=http://madmax.acme.org/images/powered_by_mandrake.png><BR>\n";

} #end sub

