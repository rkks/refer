#!/usr/bin/perl
#
# Filename: /usr/local/etc/switchupgrade.pl
#
# Language: Perl
#
# Purpose:  automated upgrading of agent and boot switch code
#           
# Author:   Michael McNamara 
#
# Date:     January 15, 2003
#
# Supported Switches:
#           - Nortel Ethernet Routing Switch 8600
#           - Nortel Ethernet Routing Switch 1600
#           - Nortel Ethernet Routing Switch 5500
#           - Nortel Ethernet Switch 470 (formerly BayStack 470)
#           - Nortel Ethernet Switch 460 (formerly BayStack 460)
#           - Nortel BayStack 450
#           - Nortel Buisness Policy Switch 
#           - HP GbE2 Switch Blade (oem'd Nortel Alteon)
#
# Requirements:
#           - Net-SNMP
#           - Net-SNMP Perl Module
#           - SNMP MIBS
#           - Net::Ping
#
# Changes:
#
#           Apr 19, 2007 (M.McNamara)
#           - added code to support SuperMezz and R module code
#           Oct 24, 2006 (M. McNamara)
#            - Added code to support switchfile as command line arg
#           Oct  8, 2006 (M. McNamara)
#            - Added date/time support and logging
#           Sep 20, 2006 (M. McNamara)
#            - Added support for BayStack 470 48T PWR Switch
#           Jun 21, 2005 (M. McNamara)
#            - Upgraded the code for the Passport 8600 switch to 3.7.7
#           Jun  8, 2005 (M. McNamara)
#            - Added code to support BayStack 5500 series switches, resolved
#              issues with upgrading BayStack 460/470 diag firmware code
#           Mar 12, 2004 (M. McNamara)
#            - Added code to automate upgrade of BayStack 450 switches
#           Mar 12, 2004 (M. McNamara)
#            - Added code to automate upgrade of BayStack 460 switches
#           Mar 1, 2004 (M. McNamara)
#            - Added code to automate upgrade of BayStack 470 switches
#	    Sept 17, 2003 (M. McNamara)
#            - Added code to incorporate all BayStack switches into backup
#           Oct 1, 2003 (M. McNamara)
#            - Added code to email status report to MLHNetAlert@mlhs.org
#            - Added perl script to weekly crontab
#
# Notes:
#           This script will not actually upgrade the ERS 8600/1600 switches.
#           It will only copy the files up to those switches, a certified 
#           eningeer will need to perform the actual upgrade since there are 
#           many variables in upgrading an 8600. This script will not warn
#           the user if the 8600 filesystem does NOT have enough room.
#
#           This script will automatically upgrade any of the remaining
#           switch models which are supported. It is highly recommended to
#           upgrade the AGENT code first before upgrading the BOOT/DIAG code.
#

# Load Modules
use strict;
use SNMP;
use Net::Ping;

# Declare constants
#use constant DEBUG      => 1;           # DEBUG settings
use constant RETRIES    => 2;           # SNMP retries
use constant TIMEOUT    => 1000000;     # SNMP timeout, in microseconds
use constant SNMPVER    => "1";         # SNMP version

# If you'd like to debug the script set this variable to 1
my $DEBUG = 0;

# Net-SNMP Engine settings
$SNMP::verbose = 0; #$DEBUG
$SNMP::use_enums = 1;
$SNMP::use_sprint_value = 1;
&SNMP::initMib();
&SNMP::loadModules('ALL');

our $community = "private";	# SNMP Read-Write community string
our $switchlist;

our @devices;
our $snmphost;
our $destination;
our $date;
our $time;

our $sysDescr;
our $sysObjectID;
our $sysUpTime;
our $sysContact;
our $sysName;
our $sysLocation;

our $FILENAME;
our $BOOT_CODE;
our $AGENT_CODE;

###########################################################################
###########################################################################
#   SET THESE ACCORDING TO THE FILENAMES YOU WISH TO USE IN THE UPGRADE   #
###########################################################################
###########################################################################
# These files need to already be loaded onto a TFTP server of your choosing
# I currently use a CentOS Linux server with all these files stored in the
# /tftpboot directory. You could also use Tftpd32 in a Win32 environment
# (http://tftpd32.jounin.net)
###########################################################################
#
# Ethernet Routing Switch 8600 Switches
#
my $BOOT_8600_CODE   = "p80b4130.img";		# Passport 8600 Boot v4.1.3
my $AGENT_8600_CODE  = "p80a4130.img";		# Passport 8600 Agent v4.1.3
my $DES_8600_CODE    = "p80c4130.img";		# Passport 8600 DES v4.1.3
my $AES_8600_CODE    = "p80c4130.aes";		# Passport 8600 AES v4.1.3
my $RMOD_8600_CODE    = "p80j4130.dld";		# Passport 8600 R Module v4.1.3
my $MEZ_8600_CODE    = "p80m4130.img";		# Passport 8600 SuperMezz v4.1.3
#
#
# BayStack 460/470 Switches
#
my $BOSS_AGENT_CODE  = "470_36612.img";		# BoSS Agent v3.6.6.1.2
my $BOSS_DIAG_CODE   = "470_3606_diag.bin";	# BoSS Diag v3.6.0.6
#
#
# BayStack 450 Switches
#
my $BOOT_450_CODE    = "b450452.img";		# BayStack 450 Diag 4.5.2
my $AGENT_450_CODE   = "b450_4546.img";		# BayStack 450 Agent 4.5.4
#
#
# BayStack 5510/5520/5530 Switches
#
my $BOOT_5500_CODE   = "5530_500003_diag.bin"; # BayStack 5500 Diag
my $AGENT_5500_CODE  = "5530_506026.img";      # BayStack 5500 Agent
#
###########################################################################
###########################################################################


our $DOAGENT = 0;			# Flag for agent upgrade
our $DOBOOT = 0;			# Flag for diag firm upgrade

my $MAILTO = "";
my $MAILFROM = "";
my $MAILSUBJECT = "Nortel BayStack 450/460/470/BPS Switch Upgrades";

my $program = "switchupgrade.pl";
my $version = "v0.75";
my $author = "Michael McNamara";
my $purpose = "This Perl script is designed to automate the upgrade of numerous Nortel Ethernet Switches and Ethernet Routing Switches.";
my $usage = "Usage: $program <agent|boot> <inputfile>\[-help\] \[debug\]\n";

if (($#ARGV +1) < 3) {
   print "Program: $program \nVersion: $version \nWritten by: $author \n$purpose\n\n$usage\n";
   print "DEBUG: ARGV =  $#ARGV\n";
   print "DEBUG: ARGV =  $ARGV[0] $ARGV[1] $ARGV[2] $ARGV[3] $ARGV[4]\n";
   die;
}

my $arg1 = shift @ARGV;
my $arg2 = shift @ARGV;
my $arg3 = shift @ARGV;

if ($arg1 =~ /-help/ ) {
   die "Program: $program \nVersion: $version \nWritten by: $author \n$purpose\n\n$usage\n";
} elsif ($arg1 =~ /-h/) {
   die "Program: $program \nVersion: $version \nWritten by: $author \n$purpose\n\n$usage\n";
} elsif ($arg1 eq "agent") {
   $DOAGENT = 1;
} elsif ($arg1 eq "boot") {
   $DOBOOT = 1;
} else {
   die "Program: $program \nVersion: $version \nWritten by: $author \n$purpose\n\n$usage\n";
}

# List of switches which we will attempt to upgrade from command line arguments
$switchlist = $arg2;

# Test to see if inputifle exists
if (!-e $switchlist) {
   die "ERROR: Unable to locate and/or open inputfile $switchlist...";
}

# If debug pass on command line arguements set debug variable
if ($arg3 eq "debug") {
   $DEBUG = 1;
}

##########################################################################
# MIBS OF INTEREST
# OIDS
# BayStack 5510 48T	sreg-BayStack5510-48T-ethSwitchNMM
# BayStack 5520 48T	sreg-BayStack5520-48T-PWR
# BayStack 304	sreg-BayStack303-304-Sw	.1.3.6.1.4.1.45.3.32.1
# BayStack 350 12/24F   sreg-BayStack350-ethSwitchNMM	.1.3.6.1.4.1.45.3.30.1
# BayStack 350 12/24T	sreg-BayStack350-24T-ethSwitchNMM	.1.3.6.1.4.1.45.3.30.2
# BayStack 450 12/24T	sreg-BayStack450-ethSwitchNMM	.1.3.6.1.4.1.45.3.35.1
# BayStack 460 24T PWR  sreg-BayStack460-24T-PWR-ethSwitchNMM
# BayStack 470 48T	sreg-BayStack470-48T-ethSwitchNMM	.1.3.6.1.4.1.45.3.46.1
# BayStack 470 24T      sreg-BayStack470-24T-ethSwitchNMM
# Business Policy Switch	sreg-BPS2000-24T-ethSwitchNMM	.1.3.6.1.4.1.45.3.40.1
# Passport 8600 Chassis	rcA8610	.1.3.6.1.4.1.2272.30

# BAYSTACK 350/450/BPS EQUIPMENT
# s5AgMyIfCfgFname.1
# BAYSTACK 470 EQUIPMENT
# June 8, 2005 I discovered that I needed to use the following MIBs for the 470/5500
# s5ChasStoreFilename.8.1.0.2 firmware diagnostics code
# s5ChasStoreFilename.8.1.0.1 agent code
# s5AgMyIfLdSvrAddr.1
# s5AgSysBinaryConfigFilename.0
# s5AgInfoFileAction
#            other(1),
#           dnldConfig(2),
#           dnldImg(3),
#           upldConfig(4),
#           upldImg(5),
#           dnldFw(6),
#           upldFw(7),
#           dnldImgIfNewer(8)
# s5AgInfoFileStatus
#            other(1) ......... if no action taken since the boot up
#            inProgress (2) ... the operation is in progress
#            success (3) ...... the operation succeeds.
#            fail (4) ......... the operation is failed."
#PASSPORT 8600 EQUIPMENT
#rcTftpHost
#rcTftpFile
#rcTftpAction
#		         none(1),          -- none of the following
#		         downloadConfig(2),
#		         uploadConfig(3),
#		         downloadSwToFlash(4),
#                         downloadSwToPcmcia(5),
#                         uploadSw(6),
#                         downloadSwToDram(7)
#rcTftpResult
#		         none(1),
#		         inProgress(2),
#		         noResponse(3),
#		         fileAccessError(4),
#		         badFlash(5),
#		         flashEraseFailed(6),
#		         pcmciaEraseFailed(7),
#		         success(8),
#		         fail(9),
#                         writeToNvramFailed(10),
#		         flashWriteFailed(11),
#		         pcmciaWriteFailed(12),
#                         configFileTooBig(13),
#                         imageFileTooBig(14),
#			 noPcmciaDetect(15),
#			 pcmciaNotSupported(16),
#			 invalidFile(17),
#			 noMemory(18),
#			 xferError(19),
#			 crcError(20),
#                         readNvramFailed(21),
#                         pcmciaWriteProtect(22)
#
#
#
# Passport 1600 Series MIBS
# snmptranslate 1.3.6.1.4.1.2272.1.201.1.1.1.2.2.0
# SWCOMMGMT-MIB::agentFirmwareSourceAddr.0
# snmptranslate 1.3.6.1.4.1.2272.1.201.1.1.1.2.4.0
# SWCOMMGMT-MIB::agentFirmwareUpdateState.0
# snmptranslate 1.3.6.1.4.1.2272.1.201.1.1.1.2.3.0
# SWCOMMGMT-MIB::agentFirmwareUpdateCtrl.0
# snmptranslate 1.3.6.1.4.1.2272.1.201.1.1.1.2.1.0
# SWCOMMGMT-MIB::agentFirmwareFile.0
#
#
##########################################################################


##########################################################################
###### M A I N    P R O G R A M ##########################################
##########################################################################

&load_switches;

&work_it;

exit;

##########################################################################
### E N D   M A I N   P R O G R A M ######################################
##########################################################################

##########################################################################
# sub work_it
#
# Purpose: work the magic and make it happen
##########################################################################
sub work_it {

   # Loop over the entire list of switches 
   foreach $snmphost (@devices) {

      ($date, $time) = &get_time;		# Date/Time 

      my $packet = Net::Ping->new('icmp');	# ICMP Ping

      $snmphost =~ s/\n//g;			# Strip CR/LF

      print "DEBUG: **********************************************\n" if ($DEBUG);
      print "DEBUG: $snmphost processing has started $date $time\n" if ($DEBUG);

      # If the switch responds to an ICMP ping continue
      if ($packet->ping($snmphost)) {

         print "DEBUG: $snmphost is responding to ICMP pings.\n" if ($DEBUG);

         # Poll the switch for its SNMP information
         if (&grab_snmpsystem == 99) {
            print "ERROR: unable to poll $snmphost via SNMP!!!!\n";
            next;
         }

         # Determine what type of switch equipment we have and proceed accordingly
         if ( ($sysObjectID eq "rcA8610") ||
              ($sysObjectID eq "rcA8606") ||
              ($sysObjectID eq "rcA1648") ) {
            #Nortel Ethernet Routing Switch 8600/1600 Switch
            &passport_tftp_upgrade;

         } elsif ( ($sysObjectID eq "sreg-BayStack5510-48T-ethSwitchNMM") ||
                ($sysObjectID eq "sreg-BayStack5510-24T-ethSwitchNMM") ||
                ($sysObjectID eq "sreg-BayStack5520-48T-PWR") ||
                ($sysObjectID eq "sreg-EthernetRoutingSwitch5530-24TFD") ) {	
            #BayStack 5520 Switch 48T
            #print "Yes, it's a BayStack 5520-48T Switch!!!\n";
            $AGENT_CODE = $AGENT_5500_CODE;
            $BOOT_CODE = $BOOT_5500_CODE;
            if ($DOAGENT) {
               # Upgrade the switch/stack agent image
               print "DEBUG: upgrading agent image...\n" if ($DEBUG);
               &baystack_tftp_upgrade;
            } elsif ($DOBOOT) {
               # Upgrade the diagnostic firmware image
               print "DEBUG: upgrading diagnostic firmware...\n" if ($DEBUG);
               &baystack_tftp_upgrade_diag;
            }

         } elsif (($sysObjectID eq "sreg-BayStack470-48T-ethSwitchNMM") || 
               ($sysObjectID eq "sreg-BayStack470-24T-ethSwitchNMM") ||
               ($sysObjectID eq "sreg-BayStack470-48T-PWR-ethSwitchNMM") ||
               ($sysObjectID eq "sreg-BayStack460-24T-PWR-ethSwitchNMM")) {
            #BayStack 470/460 Switch 48T/24T
            #print "Yes, it's a BayStack 470/460 Switch!!!\n";
            $AGENT_CODE = $BOSS_AGENT_CODE;
            $BOOT_CODE = $BOSS_DIAG_CODE;
            if ($DOAGENT) {
               # Upgrade the switch/stack agent image
               print "DEBUG: upgrading agent image...\n" if ($DEBUG);
               &baystack_tftp_upgrade;
            } elsif ($DOBOOT) {
               # Upgrade the diagnostic firmware image
               print "DEBUG: upgrading diagnostic firmware...\n" if ($DEBUG);
               &baystack_tftp_upgrade_diag;
            }
         } elsif ($sysObjectID eq "sreg-BPS2000-24T-ethSwitchNMM") {
            #Business Policy Switch
            #print "Yes, it's a Business Policy 2000 Switch!!!\n";
            $AGENT_CODE = $BOSS_AGENT_CODE;
            $BOOT_CODE = $BOSS_DIAG_CODE;
            if ($DOAGENT) {
               # Upgrade the switch/stack agent image
               print "DEBUG: upgrading agent image...\n" if ($DEBUG);
               &baystack_tftp_upgrade;
            } elsif ($DOBOOT) {
               # Upgrade the diagnostic firmware image
               print "DEBUG: upgrading diagnostic firmware...\n" if ($DEBUG);
               &baystack_tftp_upgrade_diag;
            }
         } elsif ($sysObjectID eq "sreg-BayStack450-ethSwitchNMM") {
            #BayStack 450 Switch 12/24T
            #print "Yes, it's a BayStack 450 Switch!!!\n";
            $AGENT_CODE = $AGENT_450_CODE;
            $BOOT_CODE = $BOOT_450_CODE;
            print "DEBUG: 450 upgrade agent code = $AGENT_CODE\n" if ($DEBUG);
            print "DEBUG: 450 upgrade boot code  = $BOOT_CODE\n" if ($DEBUG);
            #print "DEBUG: sleeping 30 seconds to allow ctrl-break action\n" if ($DEBUG);
            #sleep 30;
            if ($DOAGENT) {
               # Upgrade the switch/stack agent image
               print "DEBUG: upgrading agent image...\n" if ($DEBUG);
               &baystack_tftp_upgrade;
            } elsif ($DOBOOT) {
               # Upgrade the diagnostic firmware image
               print "DEBUG: upgrading diagnostic firmware...\n" if ($DEBUG);
               print "DEBUG: skipping diagnostic firmware...\n" if ($DEBUG);
               #&baystack450_tftp_upgrade_diag;
            }
         } elsif ($sysObjectID eq "sreg-BayStack350-24T-ethSwitchNMM") {
            #BayStack 350 Switch 12/24T
         } else {
            print "ERROR: $snmphost host is not tftp compatible skipping...\n";
            #print SENDMAIL "ERROR: $snmphost host is not tftp compatible skipping...\n";
         } #end if sysObjectID

      } else {

         print "ERROR: $snmphost is not responding to ICMP pings...\n";
         
      } #end if ping

   } #end foreach $snmphost

   #close(SENDMAIL);

} #end sub work_it
##########################################################################


##########################################################################
# Subroutine load_switches
#
# Purpose: load list of network switches from file into array
##########################################################################
sub load_switches {

   # Open file for input
   open(SWITCHLIST, "<$switchlist"); 

   # Walk through data file
   while (<SWITCHLIST>) {

      #print "DEBUG: (load_switches) evaluating $_ for DEVICE list\n" if ($DEBUG);

      # Skip blank lines
      next if (/^\n$/);

      # Skip comments
      next if (/^#/);

      push (@devices, $_);

      #print "DEBUG: (load_switches) adding $_ to DEVICE list\n" if ($DEBUG);

   } #end while

   # Close file
   close(SWITCHLIST);

   return;

} #end sub load_switches


##########################################################################
# Subroutine baystack_tftp_upgrade
#
# Purpose: upgrade BayStack switches agent code and diagnostic firmware
##########################################################################
sub baystack_tftp_upgrade {

# BAYSTACK 470 EQUIPMENT
# s5AgMyIfLdSvrAddr.1
# s5AgSysBinaryConfigFilename.0
# s5AgInfoFileAction
#            other(1),
#           dnldConfig(2),
#           dnldImg(3),
#           upldConfig(4),
#           upldImg(5),
#           dnldFw(6),
#           upldFw(7),
#           dnldImgIfNewer(8)
# s5AgInfoFileStatus
#            other(1) ......... if no action taken since the boot up
#            inProgress (2) ... the operation is in progress
#            success (3) ...... the operation succeeds.
#            fail (4) ......... the operation is failed."

   my $test;

   my $sess = new SNMP::Session (  DestHost  => $snmphost, 
		      		   Community => $community,
				   Version   => SNMPVER );
   my $vars = new SNMP::VarList(
			['s5AgMyIfLdSvrAddr', 1, "10.101.20.1",],
			['s5AgMyIfImgFname', 1, $AGENT_CODE,] );

   my $go = new SNMP::VarList(
                        ['s5AgInfoFileAction', 0, 3,] );

   print "DEBUG: using AGENT_CODE = $AGENT_CODE\n" if ($DEBUG);

   print "DEBUG: ** SLEEPING 5 SECONDS TO ALLOW CTRL-BREAK FOR ABORT **\n" if ($DEBUG);
   sleep 5;
 
   my $go = new SNMP::VarList(
			['s5AgInfoFileAction', 0, 3,] );

   # Set TFTP source and destination strings
   $test = $sess->set($vars);
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: SNMP set for loadserver and filename failed!\n";
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   } else {
      print "DEBUG: SNMP set for loadserver and filename successful!\n" if ($DEBUG);
   }

   # Start TFTP copy
   $test = $sess->set($go);
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: SNMP set to start TFTP download/upgrade complete!\n";
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   } else {
      print "DEBUG: SNMP set to start TFTP download/upgrade complete!\n" if ($DEBUG);
   }

   # Pause while the TFTP copy completes
   sleep 3;

   # Check to see if the TFTP copy completed
   $test = $sess->get('s5AgInfoFileStatus.0');
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: unable to poll upgrade status!\n";
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }

   # If TFTP failed output error message
   if ($test ne "success") {
        # Initially the script would wait around while the "inProgress" status
        # was reported, however, that would require that the script wait until
        # the switch rebooted which averages between 2 - 5 minutes. Now the
        # script will just report the "inProgress" status once and continue
        # to the next switch hoping that everything recovers properly.
	#while (($test eq "inProgress") | ($test eq "other")){
	   print "DEBUG: config upload status = $test (waiting)\n" if ($DEBUG);
	   sleep 3;
           $test = $sess->get('s5AgInfoFileStatus.0');
	#}
   };
   if ($test eq "fail") {
      print "ERROR: $snmphost config upload *FAILED*!\n";
      #print SENDMAIL "ERROR: $snmphost config upload *FAILED*!\n";
   } elsif ($test eq "success") {
     # print SENDMAIL "$snmphost was successful\n";
      print "$snmphost was successful\n";
   }
   print "DEBUG: download of agent image results = $test\n" if ($DEBUG);

   # Lets sleep for 15 seconds so we avoid log jamming the TFTP server
   print "DEBUG: we're NOT going to sleep here... to speed things up!\n" if ($DEBUG);
   #print "DEBUG: sleeping 10 seconds to ease load on TFTP server.\n" if ($DEBUG);
   #sleep 10;

   return 1;

} #end sub baystack_tftp_upgrade

#######################################################################
# Subroutine baystack_tftp_upgrade_diag
#
# Purpose: upgrade BayStack switches agent code and diagnostic firmware
#######################################################################
sub baystack_tftp_upgrade_diag {
   # Declare Local Variables
   my $test;

   print "DEBUG: We're in the baystack_tftp_upgrade_diag sub\n" if ($DEBUG);

   my $sess = new SNMP::Session (  DestHost  => $snmphost,
                                   Community => $community,
                                   Version   => SNMPVER );
   my $vars = new SNMP::VarList(
                        ['s5AgMyIfLdSvrAddr', 1, "10.101.20.1",],
			#mfm6/8/05 #['s5AgMyIfImgFname', 1, $BOOT_CODE,] );
			#['s5AgMyIfCfgFname', 1, $BOSS_CODE,] );
			#s5ChasStoreFilename.8.1.0.2
                        ##['s5ChasStoreFilename.8.1.0.2', , $BOOT_CODE, "OCTETSTR"] );
                        #['s5ChasStoreFilename.8.1.0', 2, $BOOT_CODE, ] );
                        ['1.3.6.1.4.1.45.1.6.3.5.1.1.8.8.1.0', 2, $BOOT_CODE, "OCTETSTR"] 
			);

   my $go = new SNMP::VarList(
                        ['s5AgInfoFileAction', 0, 6,] );

   print "DEBUG: using BOOT_CODE = $BOOT_CODE\n" if ($DEBUG);

   print "DEBUG: ** SLEEPING 5 SECONDS TO ALLOW CTRL-BREAK FOR ABORT **\n" if ($DEBUG);
   sleep 5;

   # Set TFTP source and destination strings
   $test = $sess->set($vars);
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: SNMP set for loadserver and filename failed!\n";
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   } else {
      print "DEBUG: SNMP set for loadserver and filename complete!\n" if ($DEBUG);
   }

   # Start TFTP copy
   $test = $sess->set($go);
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: SNMP set to start TFTP download/upgrade complete!\n";
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   } else {
      print "DEBUG: SNMP set to start TFTP download/upgrade complete!\n" if ($DEBUG);
   }

   # Pause while the TFTP copy completes
   sleep 3;

   # Check to see if the TFTP copy completed
   $test = $sess->get('s5AgInfoFileStatus.0');
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   } else {
      print "DEBUG: TFTP download/upgrade status = $test\n" if ($DEBUG);
   }

   # If TFTP failed output error message
   if ($test ne "success") {
        # Initially the script would wait around while the "inProgress" status
        # was reported, however, that would require that the script wait until
        # the switch rebooted which averages between 2 - 5 minutes. Now the
        # script will just report the "inProgress" status once and continue
        # to the next switch hoping that everything recovers properly.
        #while (($test eq "inProgress") | ($test eq "other")){
           print "DEBUG: config upload status = $test (waiting)\n" if ($DEBUG);
           sleep 3;
           $test = $sess->get('s5AgInfoFileStatus.0');
        #}
   };
   if ($test eq "fail") {
      print "ERROR: $snmphost config upload *FAILED*!\n";
      print SENDMAIL "ERROR: $snmphost config upload *FAILED*!\n";
   } elsif ($test eq "success") {
      print SENDMAIL "$snmphost was successful\n";
      print "$snmphost was successful\n";
   }
   print "DEBUG: download of firmware results = $test\n" if ($DEBUG);

   #print "DEBUG: sleeping 15 seconds to allow TFTP download............\n" if ($DEBUG);
   #sleep 15;

   return 1;

} #end sub baystack_tftp_upgrade_diag


##########################################################################
# Subroutine passport_tftp_upgrade
#
# Purpose: 
##########################################################################
sub passport_tftp_upgrade {

   my $test;

   my $sess = new SNMP::Session (  DestHost  => $snmphost,
                                   Community => $community,
                                   Version   => SNMPVER );
   print "DEBUG: we're in the passport tftp sub!\n" if ($DEBUG);

### 
### The SNMP MIBS below would not work for the Passport 8600
### I'm not really sure why but I decided to work around the
### problem rather than wasting all my time trying to figure
### out what the problem was/is
###

#   my $vars = new SNMP::VarList(
#                        ['rcTftpHost', 0, "10.101.20.1",],
#                        ['rcTftpFile', 0, $destination,] );
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

foreach $FILENAME ($AGENT_8600_CODE, $BOOT_8600_CODE, $DES_8600_CODE, $AES_8600_CODE)
#foreach $FILENAME ($AGENT_8600_CODE, $BOOT_8600_CODE, $DES_8600_CODE, $AES_8600_CODE, $MEZ_8600_CODE, $RMOD_8600_CODE)
{
   my $vars = new SNMP::VarList(
                        ['rc2kCopyFileSource', 0, "10.101.20.1:$FILENAME",],
                        ['rc2kCopyFileDestination', 0, "/flash/$FILENAME",] );
                        ##['rc2kCopyFileDestination', 0, "/pcmcia/$FILENAME",] );

   my $go = new SNMP::VarList(
                        ['rc2kCopyFileAction', 0, 2,] );

   # Set TFTP source and destination strings
   $test = $sess->set($vars);

   # Start TFTP copy
   $test = $sess->set($go);

   # Pause while the TFTP copy completes
   sleep 2;

   # Check to see if the TFTP copy completed
   $test = $sess->get('rc2kCopyFileResult.0');

   # If TFTP failed output error message
   if ($test ne "success") {
        while ($test eq "inProgress") {
           print "DEBUG: config upload $FILENAME status = $test (waiting)\n" if ($DEBUG);
           sleep 3;
           $test = $sess->get('rc2kCopyFileResult.0');
        }
   };
   if ($test eq "fail") {
      print "ERROR: $snmphost config upload $FILENAME *FAILED*!\n";
#      print SENDMAIL "ERROR: $snmphost config upload *FAILED*!\n";
   } elsif ($test eq "success") {
#      print SENDMAIL "$snmphost was successful\n";
      print "$snmphost was successful\n";
   }

   print "DEBUG: upload $FILENAME results = $test\n" if ($DEBUG);

} #end foreach

   return 1;

} #end sub passport_tftp_config

##########################################################################
# Subroutine grab_snmpsystem
#
# Purpose: retrieve SNMP system variables from switch device
##########################################################################
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

   ##print "DEBUG: snmphost = $snmphost and community = $community\n" if ($DEBUG);

   @vals = $sess->get($vars);   # retreive SNMP information
   if ( $sess->{ErrorStr} ) {
      print "ERROR: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }

   if ($vals[0] eq "") {
        # Let's leave the error reporting in the previous subroutine
        #print "ERROR: Unable to poll the switch $snmphost. !!!\n";
        #print SENDMAIL "ERROR: Unable to poll the switch $snmphost. !!!\n";
        return 99;
   }

   $sysDescr = $vals[0];
   $sysObjectID = $vals[1];
   $sysUpTime = $vals[2];
   $sysContact = $vals[3];
   $sysName = $vals[4];
   $sysLocation = $vals[5];

   $sysObjectID =~ s/.1.3.6.1.4.1/enterprises/;
   print "DEBUG: $snmphost sysObjectID = $sysObjectID \n" if ($DEBUG);

   return;

}; #end sub grab_snmpsystem 



#######################################################################
# Subroutine baystack450_tftp_upgrade_diag
#
# Purpose: upgrade BayStack 450 switches diagnostic boot firmware
#          the BayStack 450 uses the same MIB for both the agent
#          code and the diagnostic boot firmware code so we needed
#          a subroutine specific for the 450 diag boot code
#######################################################################
sub baystack450_tftp_upgrade_diag {
   # Declare Local Variables
   my $test;

   print "DEBUG: We're in the baystack450_tftp_upgrade_diag sub\n" if ($DEBUG);

   my $sess = new SNMP::Session (  DestHost  => $snmphost,
                                   Community => $community,
                                   Version   => SNMPVER );
   my $vars = new SNMP::VarList(
                        ['s5AgMyIfLdSvrAddr', 1, "10.101.20.1",],
			['s5AgMyIfImgFname', 1, $BOOT_CODE,] );

   my $go = new SNMP::VarList(
                        ['s5AgInfoFileAction', 0, 3, "INTEGER"] );

   # Set TFTP source and destination strings
   $test = $sess->set($vars);
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: SNMP set for loadserver and filename failed!\n";
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   } else {
      print "DEBUG: SNMP set for loadserver and filename complete!\n" if ($DEBUG);
   }

   # Start TFTP copy
   $test = $sess->set($go);
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: SNMP set to start TFTP download/upgrade complete!\n";
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   } else {
      print "DEBUG: SNMP set to start TFTP download/upgrade complete!\n" if ($DEBUG);
   }

   # Pause while the TFTP copy completes
   sleep 3;

   # Check to see if the TFTP copy completed
   $test = $sess->get('s5AgInfoFileStatus.0');
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   } else {
      print "DEBUG: TFTP download/upgrade status = $test\n" if ($DEBUG);
   }

   # If TFTP failed output error message
   if ($test ne "success") {
        # Initially the script would wait around while the "inProgress" status
        # was reported, however, that would require that the script wait until
        # the switch rebooted which averages between 2 - 5 minutes. Now the
        # script will just report the "inProgress" status once and continue
        # to the next switch hoping that everything recovers properly.
        #while (($test eq "inProgress") | ($test eq "other")){
           print "DEBUG: config upload status = $test (waiting)\n" if ($DEBUG);
           sleep 3;
           $test = $sess->get('s5AgInfoFileStatus.0');
        #}
   };
   if ($test eq "fail") {
      print "ERROR: $snmphost config upload *FAILED*!\n";
      print SENDMAIL "ERROR: $snmphost config upload *FAILED*!\n";
   } elsif ($test eq "success") {
      print SENDMAIL "$snmphost was successful\n";
      print "$snmphost was successful\n";
   }
   print "DEBUG: download of firmware results = $test\n" if ($DEBUG);

   #print "DEBUG: sleeping 15 seconds to allow TFTP download............\n" if ($DEBUG);
   #sleep 15;

   return 1;

} #end sub baystack450_tftp_upgrade_diag


#######################################################################
# Subroutine hpgbe2_upgrade
#
# Purpose: upgrade BayStack 450 switches diagnostic boot firmware
#          the BayStack 450 uses the same MIB for both the agent
#          code and the diagnostic boot firmware code so we needed
#          a subroutine specific for the 450 diag boot code
#
#-- TFTP Config
#agTftpServer OBJECT-TYPE
#    SYNTAX  DisplayString (SIZE(0..64))
#    ACCESS  read-write
#    STATUS  mandatory
#    DESCRIPTION
#        "The TFTP server IP address or domain name."
#    ::= { agTftp 1 }
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
#agTftpImageFileName OBJECT-TYPE
#    SYNTAX  DisplayString (SIZE(0..128))
#    ACCESS  read-write
#    STATUS  mandatory
#    DESCRIPTION
#        "The image file name to be downloaded."
#    ::= { agTftp 3 }
#agTftpCfgFileName OBJECT-TYPE
#    SYNTAX  DisplayString (SIZE(0..128))
#    ACCESS  read-write
#    STATUS  mandatory
#    DESCRIPTION
#        "The configuration file name."
#    ::= { agTftp 4 }
#agTftpDumpFileName OBJECT-TYPE
#    SYNTAX  DisplayString (SIZE(0..128))
#    ACCESS  read-write
#    STATUS  mandatory
#    DESCRIPTION
#        "The file name for core dump."
#    ::= { agTftp 5 }
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
#agTftpLastActionStatus OBJECT-TYPE
#    SYNTAX  DisplayString (SIZE(0..128))
#    ACCESS  read-only
#    STATUS  mandatory
#    DESCRIPTION
#        "The recorded status of the previous TFTP activity."
#    ::= { agTftp 7 }
#
#
#######################################################################
sub hpgbe2_upgrade {

   return 0;
}


########################################################################
# Subroutine get_time
#
# Purpose: figure out the current time and return as arguments
########################################################################
sub get_time {
  my ($sec, $min, $hour, $day, $mon, $year, $date, $time, $now);

  ($sec, $min, $hour, $day, $mon, $year) =  (localtime)[0,1,2,3,4,5];
  if ($sec < 10) { $sec = "0" . $sec }
  if ($min < 10) { $min = "0" . $min }
  if ($hour < 10) { $hour = "0" . $hour }
  $mon = $mon + 1;
  $year = $year + 1900;
  $date = $mon . "-" . $day . "-" . $year;
  $time = $hour . ":" . $min . ":" . $sec;
  $now = $date . " at " . $time;

  return ($date, $time);
} #end sub get_time

