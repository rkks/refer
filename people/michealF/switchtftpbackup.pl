#!/usr/bin/perl
#
# Filename: /usr/local/etc/switchtftpbackup.pl
#
# Purpose:  backup (via TFTP) the switch configuration of the switch configurations
#           to a central TFTP server for disaster recovery and documentation purposes.
#
# Author:   Michael McNamara 
#
# Date:     May 6, 2003
#
# Supported Switches:
# 	    - Nortel ERS 8600
# 	    - Nortel ERS 1600
# 	    - Nortel ERS 5500
# 	    - Nortel ES 470
# 	    - Nortel ES 460
# 	    - Nortel ES 450
# 	    - HP GbE2 Switch Blade (OEM'd Nortel Alteon)
# 	    - Motorola WS5100 (v3.x or later)
#
# Requirements:
#           - Net-SNMP
#           - Net-SNMP Perl Module
#           - SNMP MIBS
#
# Changes:
#
#           - Apr 24, 2007 (M.McNamara) 
#           added support for Motorola WS5100 3.x software release
#           - Oct 24, 2006 (M.McNamara) 
#           added support for Nortel ERS 1600 v2.x software release
#           - Sep 29, 2006 (M.McNamara) 
#           added support for BayStack 470 PwR 48T switch (added sysObjectID)
#           - Oct 20, 2005 (M.McNamara) 
#           added support for Baystack 5510 24T switch (added sysObjectID) and
#           added Ethernet Routing Switch (formerly Passport) 8600 code
#           - Mar 01, 2005 (M.McNamara)
#           incorporated a subroutine to check for the presence of the proper 
#           filename on the TFTP server (/tftpboot) thereby eliminating the 
#           first script "readytftpbackup.pl"
#           - Feb 25, 2005 (M.McNamara)
#           added the ability to retry a failed backup in TFTP subroutines
#           - Jan 13, 2004 (M.McNamara) 
#           some minor bugs throughout code base
#           - Jan 06, 2004 (M.McNamara) 
#           implemented a workaround for the Passport RAPID-CITY MIB > 3.2 problem,
#           copied OIDs for Passport 1600 into existing MIB along with required MIBS 
#           and added sub to handle ERS 1600 v1.x software
#           - Jan 05, 2004 (M.McNamara)
#           Issues with SNMP MIB for Passport 8600 v3.3.4 is presenting problems 
#           with the Net-SNMP perl modules and the old MIB cannot identify the newly 
#           added Passport 1600 switches.
#           - Dec 11, 2003 (M.McNamara)
#           Resolved issue with Passport 8600 not backing up properly
#           - Sep 17, 2003 (M.McNamara)
#           Added code to incorporate all BayStack switches into backup
#           - Oct  1, 2003 (M.McNamara)
#           Added code to email status report to email distribution list added perl 
#           script to weekly crontab
#
#

# Load Modules
use strict;
use SNMP;

# Declare constants
use constant DEBUG      => 1;           # DEBUG settings
use constant RETRIES    => 3;           # SNMP retries
use constant TIMEOUT    => 1000000;     # SNMP timeout, in microseconds
use constant SNMPVER    => 1;           # SNMP version

$SNMP::verbose = DEBUG;
$SNMP::use_enums = 1;
$SNMP::use_sprint_value = 1;
&SNMP::initMib();
&SNMP::loadModules('ALL');

our $TFTPHOST = "10.101.20.1";
our $community = "write";
our $switchlist = "/usr/local/etc/allswitches";		# list of switches
our @devices;
our $snmphost;
our $filename;

our $sysDescr;
our $sysObjectID;
our $sysUpTime;
our $sysContact;
our $sysName;
our $sysLocation;

our $sdate = `date`;

our $PAUSE = 2;

our $LASTUPDATEDDATE = "Monday November 27, 2006";

our $MAILTO = 'someone@somewhere.org';					# MAILTO
our $MAILFROM = 'someone@somewhere.org';				# MAILFROM
our $MAILSUBJECT = "Nortel Networks Switch Configuration Backups";	# MAILSUBJECT

###################################################################
### MAIN PROGRAM ##################################################
###################################################################

{
   # Load list of switches to backup
   &load_switches;

   # Start the HTML report
   &start_html_report;

   # Perform the TFTP backup
   &call_tftp_backup;

   # Finish the HTML report 
   &finish_html_report;

}

### END MAIN PROGRAM #################################################


##########################################################################
# Subroutine load_switches
#
# Purpose: load the list of switches from a file into an array
##########################################################################
sub load_switches {

   # Open file for input
   open(SWITCHLIST, "<$switchlist"); 

   # Walk through data file
   while (<SWITCHLIST>) {

      # Skip blank lines
      next if (/^\n$/);
      # Skip comments
      next if (/^#/);

      #print "DEBUG: adding $_ to our list of devices \n" if ($DEBUG);

      # Remove the CR/LF
      chomp;

      push (@devices, $_);

   } #end while

   close(SWITCHLIST);

   return 1;

} #end sub load_switches


##########################################################################
# Subroutine check_filename
#
# Purpose: make sure the filename exists with the proper permissions on
#          the TFTP server which happens to resides on the same host
##########################################################################
sub check_filename {

   # Declare Local Variables
   my $temp_filename = shift;

   print "DEBUG: I'm touching the following file /tftpboot/$temp_filename\n" if (DEBUG);
   `touch /tftpboot/$temp_filename`;
   print "DEBUG: I'm chaning the permissions on /tftpboot/$temp_filename\n" if (DEBUG);
   `chmod og-r+w /tftpboot/$temp_filename`;

   return 1;

} #end sub check_filename;


###########################################################################
# Subroutine tftp_filename
#
# Purpose: compose the TFTP filename from the FQDN of the switch
###########################################################################
sub tftp_filename {

   #Declare Local Variables
   my $host = shift;
   my $stemp;

   $stemp = $host;
   $stemp =~ s/\.mlhs\.org//g;
   $stemp =~ s/\./-/g;
   $stemp = $stemp.".cfg";

   print "DEBUG: the filename for $host will be $stemp\n" if (DEBUG);

   return $stemp;

} #end sub tftp_filename

##########################################################################
# Subroutine call_tftp_backup
#
# Purpose: determine which subroutine we should call and then execute
##########################################################################
sub call_tftp_backup {

   foreach $snmphost (@devices) {

      $snmphost =~ s/\n//g;

      #print "DEBUG: starting loop with snmphost = $snmphost\n" if (DEBUG);

      $filename = &tftp_filename($snmphost);

      &check_filename($filename);

      if (&grab_snmpsystem == 99) {
         next;
      }

      print "DEBUG: sysObjectID = $sysObjectID ($snmphost)\n";

      if ( ($sysObjectID eq "rcA8610") ||
           ($sysObjectID eq "rcA8606") ||
           ($sysObjectID eq "rcA1648") ) {
         #Ethernet Routing Switch 8600/1600
         &passport_tftp_config;
      } elsif (($sysObjectID eq "sreg-BayStack5520-48T-PWR") ||
               ($sysObjectID eq "sreg-BayStack5520-24T-PWR") ||
               ($sysObjectID eq "sreg-EthernetRoutingSwitch5530-24TFD") ||
               ($sysObjectID eq "sreg-BayStack5510-48T-ethSwitchNMM") ||
               ($sysObjectID eq "sreg-BayStack5510-24T-ethSwitchNMM")) {
         #BayStack 5500 Series Switch
         &baystack_tftp_config;
      } elsif (($sysObjectID eq "sreg-BayStack470-48T-PWR-ethSwitchNMM") || 
               ($sysObjectID eq "sreg-BayStack470-48T-ethSwitchNMM") || 
               ($sysObjectID eq "sreg-BayStack470-24T-ethSwitchNMM")) {
         #BayStack 470 Series Switch
         &baystack_tftp_config;
      } elsif ($sysObjectID eq "sreg-BayStack460-24T-PWR-ethSwitchNMM") {
         #BayStack 460 Switch 24T PWR
         &baystack_tftp_config;
      } elsif ($sysObjectID eq "sreg-BayStack450-ethSwitchNMM") {
         #BayStack 450 Switch 12/24T
         &baystack_tftp_config;
      } elsif ($sysObjectID eq "sreg-BayStack350-24T-ethSwitchNMM") {
         #BayStack 350 Switch 12/24T
         &baystack_tftp_config;
      } elsif ($sysObjectID eq "sreg-BPS2000-24T-ethSwitchNMM") {
         #Business Policy Switch
         &baystack_tftp_config;
      } elsif ($sysObjectID eq "hpProLiant-p-GbE2-InterconnectSwitch") {
         #HP Gbe2 Switch Blades
         &hpgbe2_tftp_config;
      } elsif ($sysObjectID eq "ws") {
         # Motorola/Symbol WS5100 Wireless LAN Switch v3.x Software
         &ws5100_config;
      } else {
         print "ERROR: $snmphost ~ $sysObjectID is not TFTP compatible skipping...\n";
         print SENDMAIL "<B>ERROR:</B>$snmphost ~ $sysObjectID is not tftp compatible skipping...<BR>\n";
      }

   } #end foreach $snmphost

   return 1;

} #end sub call_tftp_backup


############################################################################
# Subroutine baystack_tftp_config
#
# Purpose: use SNMP to instruct BayStack switches to TFTP upload their
# configuration file to the central TFTP server
############################################################################
sub baystack_tftp_config {

   # Declare Local Variables
   my $setresult;

   my $sess = new SNMP::Session (  DestHost  => $snmphost, 
		      		   Community => $community,
				   Version   => SNMPVER );

   my $vars = new SNMP::VarList(
			['s5AgMyIfLdSvrAddr', 1, "$TFTPHOST",],
			['s5AgMyIfCfgFname', 1, $filename,] );
 
   my $go = new SNMP::VarList(
			['s5AgInfoFileAction', 0, 4,] );

   # Set TFTP source and destination strings
   $setresult = $sess->set($vars);
   if ( $sess->{ErrorStr} ) {
      print "ERROR: {BayStack} problem setting the TFTP parameters for $snmphost\n";
      print "ERROR: {BayStack} sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }

   # Start TFTP copy
   $setresult = $sess->set($go);
   if ( $sess->{ErrorStr} ) {
      print "ERROR: {BayStack} problem setting the TFTP action bit for $snmphost\n";
      print "ERROR: {BayStack} sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }

   # Pause while the TFTP copy completes
   sleep $PAUSE;

   # Check to see if the TFTP copy completed
   $setresult = $sess->get('s5AgInfoFileStatus.0');
   if ( $sess->{ErrorStr} ) {
      print "ERROR: problem checking the TFTP result for $snmphost\n";
      print "ERROR: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }

   # If TFTP failed output error message
   if ($setresult ne "success") {
	while ($setresult eq "inProgress") {
	   print "DEBUG: config upload status = $setresult (waiting)\n" if (DEBUG);
	   sleep $PAUSE;
           $setresult = $sess->get('s5AgInfoFileStatus.0');
	} #end while
   } #end if $test ne "success"

   # If the upload command failed let's try again
   if ($setresult eq "fail") {

      print "DEBUG: initial command returned $setresult\n" if (DEBUG);
      print "DEBUG: lets try the upload command again\n" if (DEBUG);

      # Let's pause here for a few seconds since the previous command failed
      sleep $PAUSE;

      # Start TFTP copy
      $setresult = $sess->set($go);
      if ( $sess->{ErrorStr} ) {
         print "ERROR: problem setting the TFTP action bit for $snmphost\n";
         print "ERROR: sess->{ErrorStr} = $sess->{ErrorStr}\n";
      }

      # Pause while the TFTP copy completes
      sleep $PAUSE;

      # Check to see if the TFTP copy completed
      $setresult = $sess->get('s5AgInfoFileStatus.0');
         if ( $sess->{ErrorStr} ) {
            print "ERROR: problem checking the TFTP result for $snmphost\n";
            print "ERROR: sess->{ErrorStr} = $sess->{ErrorStr}\n";
      }

      # If TFTP failed output error message
      if ($setresult ne "success") {
         while ($setresult eq "inProgress") {
            print "DEBUG: config upload status = $setresult (waiting)\n" if (DEBUG);
            sleep $PAUSE;
            $setresult = $sess->get('s5AgInfoFileStatus.0');
         } #end while
      } #end if 
   } #end if

   if ($setresult eq "fail") {
      print "DEBUG: $snmphost config upload *FAILED*!\n";
      print SENDMAIL "<FONT COLOR=FF0000><B>ERROR:</B>$snmphost config upload *FAILED*!</FONT><BR>\n";
   } elsif ($setresult eq "success") {
      print SENDMAIL "$snmphost was successful<BR>\n";
      print "DEBUG: $snmphost was successful\n";
   } else {
      print "DEBUG: unknown error return = $setresult" if (DEBUG);
   } #end if

   print "DEBUG: upload config file results = $setresult\n" if (DEBUG);

   return 1;

} #end sub baystack_tftp_config

############################################################################
# Subroutine passport_tftp_config
#
# Purpose: use SNMP to instruct Passport 8600 switches to TFTP upload their
# configuration file to the central TFTP server
############################################################################
sub passport_tftp_config {

   my $test;

   my $sess = new SNMP::Session (  DestHost  => $snmphost,
                                   Community => $community,
                                   Version   => SNMPVER );
   #print "DEBUG: we're in the passport tftp sub!\n" if (DEBUG);

### 
### The SNMP MIBS below would not work for the Passport 8600
### I'm not really sure why but I decided to work around the
### problem rather than wasting all my time trying to figure
### out what the problem was/is. The trick was to initiate a
### file copy from /flash/config.cfg to the tftp server. The
### Passport 8600 natively supports copying to/from a tftp
### server (ie. copy 10.101.20.1:/image.bin /flash/image.bin)
###

#   my $vars = new SNMP::VarList(
#                        ['rcTftpHost', 0, "$TFTPHOST",],
#                        ['rcTftpFile', 0, $filename,] );
#
#   my $go = new SNMP::VarList(
#                        ['rcTftpAction', 0, 3,] );
#
#   # Set TFTP source and destination strings
#   $test = $sess->set($vars);
#   if ( $sess->{ErrorStr} ) {
#      print "DEBUG: after attempting to set the Tftp mibs we bombed!\n";
#      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
#   }
#
#   # Start TFTP copy
#   $test = $sess->set($go);
#   if ( $sess->{ErrorStr} ) {
#      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
#   }
#
#   # Pause while the TFTP copy completes
#   sleep 3;
#
#   # Check to see if the TFTP copy completed
#   $test = $sess->get('rcTftpResult.0');
#   if ( $sess->{ErrorStr} ) {
#      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
#   }

   my $vars = new SNMP::VarList(
         ['rc2kCopyFileSource', 0, "/flash/config.cfg",],
         ['rc2kCopyFileDestination', 0, "$TFTPHOST:$filename",] );

   my $go = new SNMP::VarList(
         ['rc2kCopyFileAction', 0, 2,] );

   # Set TFTP source and destination strings
   $test = $sess->set($vars);

   # Start TFTP copy
   $test = $sess->set($go);

   # Pause while the TFTP copy completes
   sleep $PAUSE;

   # Check to see if the TFTP copy completed
   $test = $sess->get('rc2kCopyFileResult.0');


   # If TFTP failed output error message
   if ($test ne "success") {
        while ($test eq "inProgress") {
           print "DEBUG: config upload status = $test (waiting)\n" if (DEBUG);
           sleep $PAUSE;
           $test = $sess->get('s5AgInfoFileStatus.0');
        } #end while
   } #end if
   if ($test eq "fail") {
      print "ERROR: $snmphost config upload *FAILED*!\n";
      print SENDMAIL "<FONT COLOR=FF0000><B>ERROR:</B>$snmphost config upload *FAILED*!</FONT><BR>\n";
   } elsif ($test eq "success") {
      print SENDMAIL "$snmphost was successful<BR>\n";
      print "DEBUG: $snmphost was successful\n";
   } #end if

   print "DEBUG: upload config file results = $test\n" if (DEBUG);

   return 1;

} #end sub passport_tftp_config

############################################################################
# Subroutine grab_snmpsystem 
#
# Purpose: use SNMP to identify the type of switch we'll be working with 
############################################################################
sub grab_snmpsystem {

   # Declare Local Variables
   my @vals;

   my $sess = new SNMP::Session (  DestHost   =>  $snmphost,
                                   Community  =>  $community,
                                   Version    =>  SNMPVER );

   my $vars = new SNMP::VarList(
                                ['sysDescr', 0],
                                ['sysObjectID', 0],
                                ['sysUpTime', 0],
                                ['sysContact', 0],
                                ['sysName', 0],
                                ['sysLocation', 0] );

   print "DEBUG: snmphost = $snmphost and community = $community\n" if (DEBUG);

   @vals = $sess->get($vars);   # retreive SNMP information
   if ( $sess->{ErrorStr} ) {
      print "ERROR: retreiving system for $snmphost\n";
      print "ERROR: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }

   if ($vals[0] eq "") {
        print "ERROR: Unable to poll the switch $snmphost. !!!\n";
        print SENDMAIL "<B>ERROR:</B>Unable to poll the switch $snmphost. !!!<BR>\n";
        return 99;
   }

   $sysDescr = $vals[0];
   $sysObjectID = $vals[1];
   $sysUpTime = $vals[2];
   $sysContact = $vals[3];
   $sysName = $vals[4];
   $sysLocation = $vals[5];

   $sysObjectID =~ s/.1.3.6.1.4.1/enterprises/;
   print "DEBUG: $snmphost sysObjectID=$sysObjectID \n" if (DEBUG);

   return 1;

}; #end sub grab_snmpsystem ########################################

############################################################################
# Subroutine passport1600_tftp_config
#
# Purpose: use SNMP to instruct Passport 1600 switches to TFTP upload their
# configuration file to the central TFTP server. This subroutine is only
# applicable to ERS 1600 switches runnning release v1.x. Any ERS 1600 series
# switches running v2.x conform to the same SNMP MIBS as the ERS 8600 switch
############################################################################
sub passport1600_tftp_config {

# swL2DevCtrlUpDownloadImageFileName
# swL2DevCtrlUpDownloadImageSourceAddr
# swL2DevCtrlUpDownloadImage OBJECT-TYPE
#                    other(1),
#                    upload(2),
#                    download(3)
# swL2DevCtrlUpDownloadState OBJECT-TYPE
#                    other(1),
#                    in-process(2),
#                    invalid-file(3),
#                    violation(4),
#                    file-not-found(5),
#                    disk-full(6),
#                    complete(7),
#                    time-out(8),
#                    tftp-establish-fail(9)
#

   my $test;

   my $sess = new SNMP::Session (	DestHost  => $snmphost,
                                        Community => $community,
                                        Version   => SNMPVER );
   my $vars = new SNMP::VarList(
       ['swL2DevCtrlUpDownloadImageSourceAddr', 0, "$TFTPHOST",],
       #['s5AgSysBinaryConfigFilename', 0, $filename,] );
       ['swL2DevCtrlUpDownloadImageFileName', 0, $filename,] );

   my $go = new SNMP::VarList(
       ['swL2DevCtrlUpDownloadImage', 0, 2,] );

   # Set TFTP source and destination strings
   $test = $sess->set($vars);
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }

   # Start TFTP copy
   $test = $sess->set($go);
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }

   # Pause while the TFTP copy completes
   print "DEBUG: Sleeping 3 seconds while TFTP backup occurs... \n" if (DEBUG);
   sleep $PAUSE;

   # Check to see if the TFTP copy completed
   $test = $sess->get('swL2DevCtrlUpDownloadState.0');
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }
   print "DEBUG: swL2DevCtrlUpDownloadState = $test \n" if (DEBUG);

   # If TFTP failed output error message
   if ($test ne "complete") {
        while ($test eq "in-process") {
           print "DEBUG: config upload status = $test (waiting)\n" if (DEBUG);
           sleep $PAUSE;
           $test = $sess->get('swL2DevCtrlUpDownloadState.0');
           print "DEBUG: swL2DevCtrlUpDownloadState = $test \n" if (DEBUG);
        }
   };

   if (($test ne "complete") & ($test ne "other"))  {
      print "DEBUG: result <> complete and <> other = $test \n" if (DEBUG);
      print "ERROR: $snmphost config upload *FAILED*!\n";
      print SENDMAIL "<FONT COLOR=FF0000><B>ERROR:</B>$snmphost config upload *FAILED*!</FONT><BR>\n";
   } elsif ($test eq "complete") {
      print SENDMAIL "$snmphost was successful<BR>\n";
      print "$snmphost was successful\n";
   }
   print "DEBUG: upload config file results = $test\n" if (DEBUG);

   return 1;

} #end sub passoprt1600_tftp_config

###########################################################################
# Subroutine strart_html_report
#
# Purpose: open handle to SENDMAIL and create HTML email report 
###########################################################################
sub start_html_report {

   open(SENDMAIL, "| /usr/lib/sendmail $MAILTO") || die;

   print(SENDMAIL "From: $MAILFROM\nTo: $MAILTO\nSubject: $MAILSUBJECT\n");
   print(SENDMAIL "MIME-Version: 1.0\n");
   print(SENDMAIL "Content-Type: text/html; charset=us-ascii\n\n");
  
   print SENDMAIL << "EOF";
<FONT FACE=VERDANA SIZE=2>
<img height="50" src="image6.gif" width="175" border="0" align="right">\<p>
<B>Network Infrastructure Switch Backup Report</B><BR>\n
Date : $sdate<BR>
<P>
<FONT FACE=VERDANA SIZE=2>
This is an automated message concerning the status of the
automated switch configuration backups for all Nortel
Ethernet Switch 450/460/470/BPS/5510/5520/5530 and Ethernet Routing Switches 8600 and 1600.\n
This message is being generated from the Fedora Core 5 Linux server running
the Perl Script switchtftpbackup.pl from /usr/local/etc.
<P>
Procedures for recovering the switch configurations can be found at this location<BR>
<A HREF=file:\\\\enterprise\\Electronics\\Documentation\\Restoring%20an%20Ethernet%20Switch%20Configuration.doc>
\\\\enterprise\\Electronics\\Documentation\\Restoring an Ethernet Switch Configuration.doc</A>
<P>
Perl Script last updated on $LASTUPDATEDDATE \n
<P>
The following is a list of the switches and their backup status. \n
<P>
EOF

return 1;
} #end sub start_html_report 

############################################################################
# Subroutine finish_html_report
#
# Purpose: close out the HTML email report
############################################################################
sub finish_html_report {

   print SENDMAIL <<EOF;
<P>
<FONT FACE=VERDANA SIZE=2>Notes: this message is now being sent in HTML format.<BR>
</font>
<P>
<IMG SRC=powered_by_perl.gif><BR>\n
<IMG SRC=powered_by_redhat.gif><BR>\n

EOF

   close(SENDMAIL);

   return 1;
} #end sub finish_html_report

############################################################################
# Subroutine hpgbe2_tftp_config
#
# Purpose: use SNMP to instruct HP GbE2 Switch Blades to TFTP upload their
# configuration file to the central TFTP server
############################################################################
sub hpgbe2_tftp_config {

#-- TFTP Config
#agTftpServer OBJECT-TYPE
#    SYNTAX  DisplayString (SIZE(0..64))
#    ACCESS  read-write
#    STATUS  mandatory
#    DESCRIPTION
#        "The TFTP server IP address or domain name."
#    ::= { agTftp 1 }
#
#agTftpImage OBJECT-TYPE
#    SYNTAX  INTEGER {
#        image1(2),
#        image2(3),
#        boot(4)
#        }
#    ACCESS  read-write
#    STATUS  mandatory
#    DESCRIPTION
#        "Whether the image file should be loaded in image1 or image2  or
#         boot in flash."
#    ::= { agTftp 2 }
#
#agTftpImageFileName OBJECT-TYPE
#    SYNTAX  DisplayString (SIZE(0..128))
#    ACCESS  read-write
#    STATUS  mandatory
#    DESCRIPTION
#        "The image file name to be downloaded."
#    ::= { agTftp 3 }
#
#agTftpCfgFileName OBJECT-TYPE
#    SYNTAX  DisplayString (SIZE(0..128))
#    ACCESS  read-write
#    STATUS  mandatory
#    DESCRIPTION
#        "The configuration file name."
#    ::= { agTftp 4 }
#
#agTftpDumpFileName OBJECT-TYPE
#    SYNTAX  DisplayString (SIZE(0..128))
#    ACCESS  read-write
#    STATUS  mandatory
#    DESCRIPTION
#        "The file name for core dump."
#    ::= { agTftp 5 }
#
#agTftpAction OBJECT-TYPE
#    SYNTAX  INTEGER {
#        other(1),
#        img-get(2),
#        cfg-get(3),
#        cfg-put(4),
#        dump-put(5)
#        }
#    ACCESS  read-write
#    STATUS  mandatory
#    DESCRIPTION
#        "This is an action object to perform various TFTP Get or Put functions.
#         The TFTP sever is specified in agTftpServer object.
#         img-get(2) - Download switch image from a specified image
#                      file (agTftpImageFileName) on the TFTP server to
#                      the destinated storage (agTftpImage).
#         cfg-get(3) - Download switch configuration from a specified
#                      file (agTftpCfgFileName) on the TFTP server.
#         cfg-put(4) - Upload switch configuration to a specified
#                      file (agTftpCfgFileName) on the TFTP server.
#         dump-put(5)- Download switch core dump to a specified
#                      file (agTftpDumpFileName) on the TFTP server.
#
#         other(1) is returned always when read."
#    ::= { agTftp 6 }
#
#agTftpLastActionStatus OBJECT-TYPE
#    SYNTAX  DisplayString (SIZE(0..128))
#    ACCESS  read-only
#    STATUS  mandatory
#    DESCRIPTION
#        "The recorded status of the previous TFTP activity."
#    ::= { agTftp 7 }
#
   my $setresult;

   my $sess = new SNMP::Session (  DestHost  => $snmphost,
                                   Community => $community,
                                   Version   => SNMPVER );

   my $vars = new SNMP::VarList(
         ['agTftpServer', 0, "$TFTPHOST",],
         ['agTftpCfgFileName', 0, "$filename",] );

   my $go = new SNMP::VarList(
         ['agTftpAction', 0, 4,] );

   # Set TFTP source and destination strings
   my $test = $sess->set($vars);

   # Start TFTP copy
   $test = $sess->set($go);

   # Pause while the TFTP copy completes
   sleep $PAUSE;

   # Check to see if the TFTP copy completed
   $test = $sess->get('agTftpLastActionStatus.0');

   # If TFTP failed output error message
   if ($test =~ /Success/) {
      print SENDMAIL "$snmphost was successful<BR>\n";
      print "DEBUG: $snmphost was successful\n";
   } else {
      print "ERROR: $snmphost config upload *FAILED*!\n";
      print SENDMAIL "<FONT COLOR=FF0000><B>ERROR:</B>$snmphost config upload *FAILED*!</FONT><BR>\n";
   } #end if

   print "DEBUG: upload config file results = $test\n" if (DEBUG);


   return 0;

} # end sub hpgbe2_tftp_config

############################################################################
# Subroutine ws5100_config
#
# Purpose: copy system:running-config to TFTP server for backup purposes
############################################################################
sub ws5100_config {

############################################################################
#  S N M P   M I B S    S Y M B O L / M O T O R O L A
############################################################################
#
#                 -- 1.3.6.1.4.1.388.14.1.2.1.5
#                wsInfraFileManage OBJECT IDENTIFIER ::= { wsInfraFileMgmtModule 5 }
#
#                -- 1.3.6.1.4.1.388.14.1.2.1.5.1
#                wsInfraFileManageSrc OBJECT-TYPE
#                        SYNTAX DisplayString
#                        MAX-ACCESS read-write
#                        STATUS current
#                        DESCRIPTION
#                                "name of the src file"
#                        ::= { wsInfraFileManage 1 }
#
#
#                -- 1.3.6.1.4.1.388.14.1.2.1.5.2
#                wsInfraFileManageDest OBJECT-TYPE
#                        SYNTAX DisplayString (SIZE (0..128))
#                        MAX-ACCESS read-write
#                        STATUS current
#                        DESCRIPTION
#                                "name of the dest file"
#                        ::= { wsInfraFileManage 2 }
#
#
#                -- 1.3.6.1.4.1.388.14.1.2.1.5.3
#                wsInfraFileManageStart OBJECT-TYPE
#                        SYNTAX INTEGER
#                                {
#                                copy(1),
#                                delete(2),
#                                rename(3),
#                                rmDir(4),
#                                mkDir(5),
#                                importKey(6),
#                                importCACert(7),
#                                importServerCert(8),
#                                importTrustPoint(9),
#                                exportKey(10),
#                                exportCert(11),
#                                exportReq(12),
#                                exportTrustPoint(13),
#                                idle(14),
#                                computeSavedCfgChkSum(15),
#                                computeRunningCfgChkSum(16),
#                                abort(17)
#                                }
#                        MAX-ACCESS read-write
#                        STATUS current
#                        DESCRIPTION
#                                "local file management start, setting this value to 1 means copy,
#                                2 means delete, 3 means rename, 4 means rmdir, 5 mean mkdir, and
#                                when there is no local file management, the back end
#                                will set this value to idle.
#                                copy/rename, both src and dest need to be specified
#                                delete/rmdir/mkdir, only src needs to be specified
#
#                                Files: flash:/path/file
#                                       nvram:startup-config
#                                       system:running-config
#
#                                URLs: tftp://<hostname or IP>/path/file
#                                      ftp://<user>:<passwd>@<hostname or IP>/path/file
#                                      scp://<user>@<hostname or IP>/path/file
#
#                                "
#                        ::= { wsInfraFileManage 3 }
#
#
#                -- 1.3.6.1.4.1.388.14.1.2.1.5.4
#                wsInfraFileManageStatus OBJECT-TYPE
#                        SYNTAX DoActionState
#                        MAX-ACCESS read-only
#                        STATUS current
#                        DESCRIPTION
#                                "state of the file manage, success,failure, or inProgress"
#                        ::= { wsInfraFileManage 4 }
#
#
#                -- 1.3.6.1.4.1.388.14.1.2.1.5.5
#                wsInfraFileManageLastFailedReason OBJECT-TYPE
#                        SYNTAX DisplayString
#                        MAX-ACCESS read-only
#                        STATUS current
#                        DESCRIPTION
#                                "file manage last failed reason."
#                        ::= { wsInfraFileManage 5 }
#
#
####################################################################################

   my $setresult;

   my $sess = new SNMP::Session (  DestHost  => $snmphost,
                                   Community => $community,
                                   Version   => SNMPVER );

   my $vars = new SNMP::VarList(
         ['wsInfraFileManageSrc', 0, "system:running-config",],
         ['wsInfraFileManageDest', 0, "tftp://$TFTPHOST/$filename",] );

   my $go = new SNMP::VarList(
         ['wsInfraFileManageStart', 0, 1,] );

   # Set TFTP source and destination strings
   my $test = $sess->set($vars);

   # Start TFTP copy
   $test = $sess->set($go);

   # Pause while the TFTP copy completes
   sleep $PAUSE;

   # Check to see if the TFTP copy completed
   $test = $sess->get('wsInfraFileManageStatus.0');

   # If TFTP failed output error message
   if ($test =~ /success/) {
      print SENDMAIL "$snmphost was successful<BR>\n";
      print "DEBUG: $snmphost was successful\n";
   } else {
      print "ERROR: $snmphost config upload *FAILED*!\n";
      print SENDMAIL "<FONT COLOR=FF0000><B>ERROR:</B>$snmphost config upload *FAILED*!</FONT><BR>\n";
   } #end if

   print "DEBUG: upload config file results = $test\n" if (DEBUG);

   return 0;

} #end sub ws5100_config

