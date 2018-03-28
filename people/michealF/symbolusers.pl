#!/usr/bin/perl
#
# Filename: /var/www/cgi-bin/symbolusers.pl
#
# Purpose:  Perl CGI Script to query Symbol Wireless Switch and output
#           information regarding current users and associated MUs.
#
# Author:   Michael McNamara
#
# Date:     March 24, 2004
#
# Changes:
#           July 6, 2005 (M.McNamara)
#           Removed various MIB variables from SNMP query to speed application
#           April 20, 2004 (M.McNamara)
#           Added additional information about APs including location text.
#           September 9, 2004 (M. McNamara)
#           Added additional information about "Standby" Wireless switches
#

# Load Modules
use strict;
use SNMP;
use CGI;
use HTML::Template;
use Socket;

# Declare constants
my $date = localtime;
my $author = "Michael McNamara";
my $lastchange = "January 10, 2005";

#use constant DEBUG      => 1;           # DEBUG settings
use constant RETRIES    => 3;            # SNMP retries
use constant TIMEOUT    => 10000000;     # SNMP timeout, in microseconds
use constant SNMPVER    => "1";          # SNMP version

# Declaration Variables
my $number_aps = 0;			# Number of Access Ports detected
my $number_devices = 0;			# Number of wireless devices detected

our ($sess, @vals);
our @devices;
our @radio;
our $switch;
our $comm = "public";
our %array;

my %radioName;
my %radioLocation;
my %radioStatus;

our ($days, $hrs, $mins, $tTime);				# Time Variables
our $intf;
our ($sysDescr, $sysObjectID, $sysUpTime, $sysContact, 		# SNMP Variables
	$sysName, $sysLocation);
our ($HAmode, $HAstate, $HAstatus, $HAfail);			# Symbol HA Status

my @aploop;			# Hash for looping over APs
my @muloop;			# Hash for looping over MUs

############################################

my $DEBUG = 0;		# DEBUG FLAG used for troubleshooting

# SNMP Settings
$SNMP::verbose = $DEBUG;
$SNMP::debugging = 1;
$SNMP::use_enums = 1;
$SNMP::use_sprint_value = 0;
&SNMP::addMibDirs('/usr/share/snmp/mibs');
&SNMP::loadModules('ALL');
&SNMP::initMib();

undef @devices;				# Empty the array
$| = 1;					# Disable IO buffering
my $web = new CGI;			# CGI Object
my $template = HTML::Template->new(     # HTML::Template
                filename => "symbolusers.tmpl",
                die_on_bad_params => 0
                );
$template->param
(
    TITLE => "Technology Infrastructure",
    KEYWORDS => "symbol, 802.11b, wireless, perl",
    DESCRIPTION => "List active users on Symbol Wireless LAN Switch",
);

######################################################################
# Symbol Wireless Switch 5000
######################################################################
#CcMUInfoEntry ::=
#        SEQUENCE {
#                ccMUInfoIndex           Integer32,
#                ccMUInfoType            MUDeviceType,
#                ccMUInfoMac             DisplayString,
#                ccMUInfoIP              DisplayString,
#                ccMUInfoWlan            DisplayString,
#                ccMUInfoEssid           DisplayString,
#                ccMUInfoAP              DisplayString,
#                ccMUInfoAPState         APStatus,
#                ccMUInfoSecState        MUSecurityStatus,
#                ccMUInfoCurRate         CurrentRate,
#                ccMUInfoSupRates        SupportedRates,
#                ccMUInfoRssi            DisplayString,
#                ccMUInfoPsp             PSPowerMode,
#                ccMUInfoTktExp          DisplayString,
#                ccMUInfoUserName        DisplayString,
#                ccMUInfoPktTx           DisplayString,
#                ccMUInfoPktRx           DisplayString,
#                ccMUInfoBytesTx         DisplayString,
#                ccMUInfoBytesRx         DisplayString,
#                ccMUInfoLastAct         DisplayString,
#                ccMUInfoVlan            DisplayString,
#                ccMUInfoAuthState       AuthState,
#                ccMUInfoAuthMethod      AuthMethod,
#                ccMUInfoUniEncrType     EncrType,
#                ccMUInfoBCMCEncrType    EncrType
#
#CcAPEntry ::=
#        SEQUENCE {
#                ccRadioIndex                    INTEGER,
#                ccRadioName                     DisplayString,
#                ccRadioDesc                     DisplayString,
#                ccRadioOnlineStatus             APOnlineStatus,
#                ccRadioMAC                      DisplayString,
#                ccDeviceMAC                     DisplayString,
#                ccDeviceLocation                DisplayString,
#                ccRadioType                     DisplayString,
#                ccRadioChannel                  DisplayString,
#                ccRadioPower                    DisplayString,
#                ccRadioPolicy                   DisplayString,
#                ccDeviceNic                     INTEGER,
#                ccDeviceType                    DisplayString,
#                ccRadioCCAmode                  INTEGER,
#                ccRadioCCAthresh                INTEGER,
#                ccRadioDiversity                INTEGER,
#--              ccAccessPortSo                  INTEGER,
#--              ccAccessPortSoList              DisplayString,
#--              ccAccessPortGraph               INTEGER,
#--              ccAccessPortGraphList           DisplayString,
#                ccDeviceVlanid                  INTEGER,
#                ccDeviceVlanTagsSeen            DisplayString,
#                ccRadioDiversity                INTEGER,
#                ccRadioUptime                   DisplayString,
#                ccRadioTxpps                    INTEGER,
#                ccRadioMUs                      INTEGER,
#                ccRadioGatherStatistics         TruthValue,
#                ccRadioReset                    TruthValue,
#                ccDeviceClearSeenVlanTags       TruthValue,
#                ccDeviceReset                   TruthValue,
#                ccRadioAuto                     TruthValue
#        }
#
################################################################
# Nortel Networks Wireless Security Switch 2270 (Airespace)
################################################################
#BsnAPEntry ::=
#    SEQUENCE {
#    bsnAPDot3MacAddress            MacAddress,
#    bsnAPNumOfSlots                INTEGER,
#    bsnAPName                      OCTET STRING,
#    bsnApIpAddress                 IpAddress,
#    bsnAPLocation                  OCTET STRING,
#    bsnAPMonitorOnlyMode           INTEGER,
#    bsnAPOperationStatus           INTEGER,
#    bsnAPSoftwareVersion           DisplayString,
#    bsnAPBootVersion               DisplayString,
#    bsnAPPrimaryMwarName           OCTET STRING,
#    bsnAPReset                     INTEGER,
#    bsnAPStatsTimer                INTEGER,
#    bsnAPPortNumber                INTEGER,
#    bsnAPModel                     DisplayString,
#    bsnAPSerialNumber              DisplayString,
#    bsnAPClearConfig               INTEGER,
#    bsnAPMirrorMode                INTEGER,
#    bsnAPRemoteModeSupport         INTEGER,
#    bsnAPAdminStatus               INTEGER
#    }
#
#BsnMobileStationEntry ::=
#    SEQUENCE {
#        bsnMobileStationMacAddress                       MacAddress,
#        bsnMobileStationIpAddress                        IpAddress,
#        bsnMobileStationUserName                         DisplayString,
#        bsnMobileStationAPMacAddr                      MacAddress,
#        bsnMobileStationAPIfSlotId                     INTEGER,
#        bsnMobileStationEssIndex                         INTEGER,
#        bsnMobileStationSsid                             DisplayString,
#        bsnMobileStationAID                              Unsigned32,
#        bsnMobileStationStatus                           INTEGER,
#        bsnMobileStationReasonCode                       INTEGER,
#        bsnMobileStationMobilityStatus                   INTEGER,
#        bsnMobileStationAnchorAddress                    IpAddress,
#        bsnMobileStationCFPollable                       INTEGER,
#        bsnMobileStationCFPollRequest                    INTEGER,
#        bsnMobileStationChannelAgilityEnabled            INTEGER,
#        bsnMobileStationPBCCOptionImplemented            INTEGER,
#        bsnMobileStationShortPreambleOptionImplemented   INTEGER,
#        bsnMobileStationSessionTimeout                   Unsigned32,
#        bsnMobileStationAuthenticationAlgorithm          INTEGER,
#        bsnMobileStationWepState                         INTEGER,
#        bsnMobileStationPortNumber                       Unsigned32
#        bsnMobileStationDeleteAction                     INTEGER,
#        bsnMobileStationPolicyManagerState               DisplayString,
#        bsnMobileStationSecurityPolicyStatus             INTEGER,
#        bsnMobileStationProtocol                         INTEGER,
#        bsnMobileStationMirrorMode                       INTEGER,
#        bsnMobileStationStatusCode                       INTEGER




##############################################################################
#   B  E  G  I  N    M  A  I  N
##############################################################################

&initialize_cgi;

if ($switch eq "") {				# Display Form
   $template->param(SUBMIT => 1);

} else {					# Execute Script
   $template->param(SUBMIT => 0);

   &get_syssystem;				# Retreieve SNMP variables
   &get_ap;					# Retreieve list of APs
   &check_ap;					# Check if APs are online
   &get_main;					# Retreieve list of MUs
}

$template->param( NUMBER_APS => $number_aps);
$template->param( NUMBER_MUS => $number_devices);

print $web->header();				# Write HTML header
print $template->output();			# Write HTML body

exit 0;
##############################################################################
#   E  N  D     M  A  I  N
##############################################################################

##############################################################################
# Subroutine initialize_cgi
#
# Purpose: initialize the CGI environment and retreive CGI variables
##############################################################################
sub initialize_cgi {

   $switch = $web->param('switch');
   $DEBUG = $web->param('debug');

   $template->param( DATE => $date );
   $template->param( AUTHOR => $author );
   $template->param( LASTCHANGE => $lastchange );
   $template->param( DEBUG => $DEBUG );
   $template->param( SWITCH => $switch );

   $template->param(DEBUG => $DEBUG);

   return 1;
}; # end sub

##########################################################################

##############################################################################
sub get_syssystem {

   $switch =~ s/\n//g;	# remove line feed

   $sess = new SNMP::Session (	DestHost   =>  $switch, 
				Community  =>  $comm,
				Retry      =>  RETRIES,
				Timeout    =>  TIMEOUT,
				Version    =>  SNMPVER );

   our $vars = new SNMP::VarList(
				['sysDescr', 0],
				['sysObjectID', 0],
				['sysUpTime', 0],
				['sysContact', 0],
				['sysName', 0],
				['sysLocation', 0],
				['ccHsbFailoverState', 0],
				['ccHsbMode', 0],	# Primary, Standby
			 	['ccHsbEnabled', 0],	# Enabled, Disabled
				['ccHsbFailoverReason', 0]);
   
   @vals = $sess->get($vars);	# retreive SNMP information
   if ( $sess->{ErrorStr} ) {
      print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
   }

   $sysDescr = $vals[0];
   $sysObjectID = $vals[1];
   $sysUpTime = $vals[2];
   $sysContact = $vals[3];
   $sysName = $vals[4];
   $sysLocation = $vals[5];
   $HAstate = $vals[6];
   $HAmode = "<FONT COLOR=FF0000><B>".$vals[7]."</B></FONT>";
   $HAstatus = $vals[8];
   $HAfail = $vals[9];

#High Availability Standby Mode   = <TMPL_VAR NAME="HAMODE"><BR>  ccHsbMode
#High Availability Standby Status = <TMPL_VAR NAME="HASTATUS"><BR> ccHsbFailoverState
#High Availability Standby State  = <TMPL_VAR NAME="HASTATE"><BR> ccHsbEnabled
#High Availability Standby Failover reason = <TMPL_VAR NAME="HAFAIL"><BR> ccHsbFailoverReason
   $template->param( HAMODE => $HAmode);
   $template->param( HASTATUS => $HAstatus);
   $template->param( HASTATE => $HAstate);
   $template->param( HAFAIL => $HAfail);

   if ($HAstate eq "monitoring") {
      $template->param(STANDBY => 1);
   } else {
      $template->param(STANDBY => 0);
   }

   $sysObjectID =~ s/.1.3.6.1.4.1/enterprises/;

   return 0;
};

#############################################################################
# Subroutine get_ap
#
# Purpose: walk the AP entry table and store for reference
#############################################################################
sub get_ap {

   #Declare Local Variables
   my $intf = 0;

   while (1) {

      $intf++;			# Index for SNMP walk 

      my $vars = new SNMP::VarList (
                ['ccRadioName', $intf],
                ['ccRadioOnlineStatus', $intf],
#                ['ccRadioMAC', $intf],
                ['ccDeviceLocation', $intf]
#                ['ccRadioChannel', $intf],
#                ['ccRadioPower', $intf],
#                ['ccRadioUptime', $intf],
#                ['ccRadioTxpps', $intf],
#                ['ccRadioMUs', $intf],
#                ['ccRadioGatherStatistics', $intf],
#                ['ccRadioReset', $intf],
#                ['ccDeviceReset', $intf],
                );

      @vals = $sess->get($vars);        # retreive SNMP information
      #if ( $sess->{ErrorStr} ) {
      #   print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
      #}

      last if ($vals[0] eq "");

      $number_aps++;		# Count number of APs

      if ($DEBUG) {
         print "ccRadioName              = <b>$vals[0]</b><br> \n";
         print "ccRadioOnlineStatus      = <b>$vals[1]</b><br> \n";
         print "ccRadioMAC               = <b>$vals[2]</b><br> \n";
         print "ccDeviceLocation         = <b>$vals[3]</b><br> \n";
         print "ccRadioUptime            = <b>$vals[4]</b><br> \n";
         print "ccRadioTxpps             = <b>$vals[5]</b><br> \n";
         print "ccRadioMUs               = <b>$vals[6]</b><br> \n";
         print "ccRadioGatherStatistics  = <b>$vals[7]</b><br> \n";
         print "ccRadioReset             = <b>$vals[8]</b><br> \n";
         print "ccDeviceReset            = <b>$vals[9]</b><br> \n";
         print "<p>\n";
      }

      #$radioLocation{$vals[0]} = $vals[3];
      $radioLocation{$vals[0]} = $vals[2];
      $radioStatus{$vals[0]} = $vals[1];

   } #end while

   if ($DEBUG) {
      for my $index (keys %radioLocation) {
         print "802.11b Radio $index is located at $radioLocation{$index}<br>\n";
      }
   }

   return 1;
}

##############################################################################
# Subroutine get_main
#
# Purpose: this is the meat and potatoes of the script 
##############################################################################
sub get_main {

   # Declare Local Variables
   my $intf = 0;
   my $font = "<FONT FACE=TAHOMA SIZE=2>";
   my $dnsname;

   while (1) {

      $intf++;				# Index for SNMP walks

      my $vars = new SNMP::VarList(
			['ccMUInfoIndex', $intf],		# 0 0
			#['ccMUInfoType', $intf],		# 1
			['ccMUInfoMac', $intf],			# 2 1
			['ccMUInfoIP', $intf],			# 3 2
			['ccMUInfoWlan', $intf],		# 4 3
			['ccMUInfoEssid', $intf],		# 5 4
			['ccMUInfoAP', $intf],			# 6 5
			#['ccMUInfoAPState', $intf],		# 7
			#['ccMUInfoSecState', $intf],		# 8
			#['ccMUInfoCurRate', $intf],		# 9
			#['ccMUInfoSupRates', $intf],		# 10
			['ccMUInfoRssi', $intf],		# 11 6
			#['ccMUInfoPsp', $intf],			# 12
			#['ccMUInfoIntf', $intf],		# 13
			['ccMUInfoAsscUptime', $intf],		# 14 7
			#['ccMUInfoTktExp', $intf],		# 15
			['ccMUInfoUserName', $intf],		# 16 8
			#['ccMUInfoPktTx', $intf],		# 17
			#['ccMUInfoPktRx', $intf],		# 18
			#['ccMUInfoBytesTx', $intf],		# 19
			#['ccMUInfoBytesRx', $intf],		# 20
			['ccMUInfoLastAct', $intf],		# 21 9
			#['ccMUInfoVlan', $intf],		# 22
			['ccMUInfoAuthState', $intf],		# 23 10
			#['ccMUInfoAuthMethod', $intf],		# 24
			#['ccMUInfoUniEncrType', $intf],		# 25
			['ccMUInfoBCMCEncrType', $intf] );	# 26 11

      @vals = $sess->get($vars);	# retreive SNMP information
      #if ( $sess->{ErrorStr} ) {
      #   print "DEBUG: sess->{ErrorStr} = $sess->{ErrorStr}\n";
      #}

      last if ($vals[0] eq "");
      
      $number_devices++;		# Counter for number of MUs

      # Let's perform a DNS lookup to get the FQDN of the MU
      $dnsname = ( gethostbyaddr (inet_aton ($vals[2]), AF_INET) || "UNRESOLVED" );
      $dnsname =~ s/\.local\.domain\.root//g;	# Let's drop the FQDN from the hostname

      my %murow = (  INDEX => $vals[0],
		MUMAC => $vals[1],
		MUIP  => $vals[2],
		WLAN  => $vals[3],
		ESSID => $vals[4],
		APMAC => $vals[5],
		APLOC => $radioLocation{$vals[5]},
		USER  => $vals[8],
		###APSTATE => $vals[7],
		AUTHSTATE => $vals[10],
		ENCRTYPE  => $vals[11],
		RSSI      => $vals[6],
		ASOCTIME  => &timefromsec($vals[7]),
		LASTACT   => &timefromsec($vals[9]),
		DNSHOST   => $dnsname
		);
		
       # put this row into the loop by reference             
       push(@muloop, \%murow);
		
      } # end while

   $template->param( USERLOOP => \@muloop );

   return 1;
}

###########################################################
# Subroutine calc_down_time
#
# Purpose: calculate downtime given to time references
###########################################################
sub timefromsec {
   # Declare Local Variables
   my $cTime = shift;         
                           
   my $iTime;                  # days:hours:minutes:seconds

   my ($iDays, $iHours, $iMins, $iSecs);

   # Do the math
   if ($cTime != 0) {
      $iTime = $cTime;

      $iSecs   = $iTime % 60;
      $iTime  -= $iSecs;

      $iMins   = $iTime % 3600;
      $iTime  -= $iMins;
      $iMins  /= 60;

      $iHours  = $iTime % 86400;
      $iTime  -= $iHours;
      $iHours /= 3600;

      $iDays  = $iTime / 86400;

      if ($iSecs < 10) { $iSecs = "0" . $iSecs }
      if ($iMins < 10) { $iMins = "0" . $iMins }
      if ($iHours < 10) { $iHours = "0" . $iHours }

      # Put it all together
      $iTime = "$iDays:$iHours:$iMins:$iSecs";

   } else {
      $iTime = "0:00:00:00";
   }

   return $iTime;
} #end sub timefromsec



############################################################################
# Subroutine check_ap
# 
# Purpose: check that all Access Points are online and operational and
#          alert the operator if there is a problem detected
############################################################################ 
sub check_ap {

   # Declare Local Variables
   my $test;

   for my $index (keys %radioStatus) {
      if ($radioStatus{$index} eq "unavailable") {
         $template->param(APHELP => 1);
         my %aprow = (	INDEX => $index,
			APLOC => $radioLocation{$index});

         # put this row into the loop by reference             
         push(@aploop, \%aprow);
      }

   }

   $template->param(APLOOP => \@aploop);

   return 1;

} #end sub check_ap
