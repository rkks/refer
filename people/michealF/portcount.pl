#!/usr/bin/perl
#
# Filename: /usr/local/etc/port/portcount.pl
#
# Purpose:  Query Nortel switches for ifInOctets to provide port capacity
#           utilization statistics. We'll keep a running count of the
#           ifInOctets for each MIB2 interface and report on how long the
#           port has been idle (meaning we saw no change in the counter).
#           
#
# Switches: Nortel Passport 8600
#           Nortel Passport 1600
#           Nortel BayStack 470
#           Nortel BayStack 470 PoE (PwR)
#           Nortel BayStack 460 PoE (PwR)
#           Nortel BayStack 450
#           Nortel BayStack 350
#           Nortel Business Policy Switch
#           Nortel BayStack 5510 
#           Nortel BayStack 5520 PoE (PwR)
#           Nortel BayStack 5530
#           HP GbE2 Switch Blades
#           Cisco Catalyst 2950, 2960, 3560
#
# Author:   Michael McNamara (mfm@michaelfmcnamara.com)
#
# Credits:  Stewart Kendric (http://www.skendric.com)
#           
# Date:     July 28, 2003
#
# Version:  1.5
#
# Changes:
#
#  August 13, 2010 (M.McNamara)
#    added support for Cisco 3750E Stacks, also missing 3560E models
#  June 17, 2010 (M.McNamara)
#    added support for HP GbE2c
#  November 19, 2009 (M.McNamara)
#    added support for Cisco Catalyst 2950, 2960, 3560, 6500 switches
#    added $ifType eq "other" to skip non-physical interfaces
#  August, 31, 2009 (M.McNamara)
#    added $ifType eq "l2vlan" $ifType eq "l3ipvlan" $ifType eq "propVirtual"
#    to help clean up the non-physical interfaces from the reports
#  May 9, 2007 (M.McNamara)
#    added prune_switches sub to query only devices this script supports
#  October 16, 2006 (M.McNamara)
#    added code to skip cascasde ports in ifTable on switch stacks
#    "next if ($ifType eq "propMultiplexor");"
#  October  2, 2006 (M.McNamara)
#    added support for BayStack 470 PwR
#  December 6, 2005 (M.McNamara)
#    added support for BayStack 5530 24FTD switch
#    added support for HP GbE2 switch blades (v2.2)
#  December 30, 2004 (M.McNamara)
#    added support for BayStack 5520 PWR switch
#  August 23, 2004 (M.McNamara)
#    added suppor for BayStack 5510 48T switch
#  March 14, 2004 (M. McNamara)
#    added support for BayStack 460 24T PWR swtich
#  January 21, 2004 (M. McNamara)
#    added debug command line paramaters
#  January 13, 2004 (M. McNamara)
#    change the SNMP version from "2c" to "1" in order to support older equipment
#    adopted the byKeys sub to sort DNS names and then slot/port numbers in order
#    to sort and clean up the output of the hash array
#    added routines to support BayStack 303/304 and BayStack 350F switches
#  January 07, 2004 (M.McNamara)
#    Added support for Passport 1600 switches and corrected some bugs
#  Dec 30, 2003: added ICMP ping check before any SNMP queries
#  Dec 17, 2003: added fixes for Passport 1648T switch
#  Dec 01, 2003: added routines to check if switch is reachable
#  Oct 01, 2003: added fixes for BayStack 24T switch
#
# Load Modules
use strict;
use SNMP;
use Net::Ping;

# Declare constants
#use constant DEBUG      => 1;           # DEBUG settings
use constant RETRIES    => 3;           # SNMP retries
use constant TIMEOUT    => 1000000;     # SNMP timeout, in microseconds
use constant SNMPVER    => 1;           # SNMP version

my $DEBUG = 1;		# DEBUG Flag for troubleshooting

# Setup SNMP Variables
$SNMP::verbose = $DEBUG;
$SNMP::use_enums = 1;
$SNMP::use_sprint_value = 1;
&SNMP::initMib();
&SNMP::loadModules('ALL');
#&SNMP::loadModules('+RAPID-CITY');
#&SNMP::loadModules('+SYNOPTICS-ROOT-MIB');

# Declare Hashes, Arrays and Scalar Variables
my %disk;		# Data Structure from disk
my %live;		# Data Structure from snmpget
my %data;		# Data Structure for merge
my %totalPorts;		# Hash for total port counts
my %activePorts;	# Hash for list of active ports
my %idlePorts;		# Hash for list of idle ports
my $totalPorts;		# Counter for total ports
my $activePorts;	# Counter for total active ports
my $idlePorts;		# Counter for total idle ports

my $goneDays;
my $idleDays;
my $idleTime;
my $dataFile;
my ($sess, @vals);
my @found;
my ($card, $port);
my $community = "5ninety";

# Local Data Files
my $switchlist = "/usr/local/etc/allswitches";		# List of switches
#my $switchlist = "/usr/local/etc/port/test1.txt";	# List of switches
my $portDataFile = "/usr/local/etc/port/portdata.txt";	# Datafile
my $summaryFile = "/usr/local/etc/port/portsummary.txt";	# Summary Report file
my $reportFile = "/usr/local/etc/port/portreport.txt";	# Detailed Report file

our @devices;
our $snmphost;
my %list;
my %list1;
my $index,
my $descr;
my $status;
my $currTime;
my $date;
my $time;
our ($sysObjectID, $sysName, $sysDescr, $sysUpTime, $sysContact, $sysLocation, $sysSoftware);

our @sysObjectIDs = qw/
      rcA8610
      rcA8606
      rcA1648
      sreg-EthernetRoutingSwitch5530-24TFD
      sreg-BayStack5520-48T-PWR
      sreg-BayStack5520-24T-PWR
      sreg-BayStack5510-48T-ethSwitchNMM
      sreg-BayStack5510-24T-ethSwitchNMM
      sreg-ERS-4548GT-PWR
      sreg-BayStack470-48T-PWR-ethSwitchNMM
      sreg-BayStack470-48T-ethSwitchNMM
      sreg-BayStack470-24T-ethSwitchNMM
      sreg-BayStack460-24T-PWR-ethSwitchNMM
      sreg-BayStack450-ethSwitchNMM
      sreg-BayStack350-24T-ethSwitchNMM
      sreg-BayStack350-ethSwitchNMM
      sreg-BPS2000-24T-ethSwitchNMM
      sreg-BayStack303-304-Sw
      sreg-SMB-BES-50-GE-24T-PWR
      hpProLiant-p-GbE2-InterconnectSwitch
      hpProLiant-GbE2c-InterconnectSwitch
      catalyst6kMsfc2
      catalyst295012
      catalyst2924XLv
      catalyst2924MXL
      catalyst2950t24
      catalyst295048G
      catalyst295024G
      catalyst296024
      catalyst296048TT
      catalyst2960G48
      catalyst356024PS
      catalyst3560G48TS
      catalyst3560E24PD
      catalyst3560G24PS 
      catalyst3560G48PS
      catalyst37xxStack
/;

our @AvayaERSSwtichStacks = qw/
      sreg-EthernetRoutingSwitch5530-24TFD
      sreg-BayStack5520-48T-PWR
      sreg-BayStack5520-24T-PWR
      sreg-BayStack5510-48T-ethSwitchNMM
      sreg-BayStack5510-24T-ethSwitchNMM
      sreg-ERS-4548GT-PWR
      sreg-BayStack470-48T-PWR-ethSwitchNMM
      sreg-BayStack470-48T-ethSwitchNMM
      sreg-BayStack470-24T-ethSwitchNMM
      sreg-BayStack460-24T-PWR-ethSwitchNMM
      sreg-BayStack450-ethSwitchNMM
      sreg-BayStack350-24T-ethSwitchNMM
      sreg-BayStack350-ethSwitchNMM
      sreg-BPS2000-24T-ethSwitchNMM
/;


#
##
###
##
#

# SNMP OIDs
my $ifstatus = "interfaces.ifTable.ifEntry.ifOperStatus.";
my $ifdesc = "interfaces.ifTable.ifEntry.ifDescr.";
my $iftotal = "ifNumber.0";

# Timing
$idleDays    = 45;
$goneDays    = 10;
$idleTime    = time - $idleDays*24*60*60;
$currTime    = time;
($date, $time) = &get_time;

# Define global variables
$| = 1;                        # Disable buffering of IO

# Stanity check for troubleshooting purposes
#print "$currTime\n";
#print "Date = $date & Time = $time\n";

my $program = "portcount.pl";
my $version = "v0.1f";
my $author = "Michael McNamara";
my $purpose = "This Perl script is designed to poll the network electronics via SNMP and provide a report of which Ethernet switch ports are idle and can be re-used for another device.";
my $usage = "Usage: $program \[debug\]\n";

my $test = shift @ARGV;

if ($test =~ /-help/ ) {
   die "Program: $program \nVersion: $version \nWritten by: $author \n$purpose\n\n$usage\n";
} elsif ($test =~ /-h/) {
   die "Program: $program \nVersion: $version \nWritten by: $author \n$purpose\n\n$usage\n";
} elsif ($test eq "debug") {
   $DEBUG = 1;
   print "DEBUG: setting DEBUG flag to enable\n";
}


############################################################
# Main Program
############################################################

&loaddevices;	 # Load hostnames of all switches

&prune_switches; # Remove devices we don't support

&read_data;	 # Load all previously stored data

&getsnmpdata;	 # Gather all SNMP data

&compare_counts; # Compare the current and stored data

&count_ports;	 # Count the number of ports for reporting

&write_data;	 # Save the new data for furture use

&write_report;	 # Output the idle port report

&write_summary;	 # Output the summary report

#############################################################
# End Main Program
#############################################################


#############################################################
# Subroutine to load list of targes (switches)
#############################################################
sub loaddevices {

   open(SWITCHLIST, "<$switchlist");

   # Walk through data file
   while (<SWITCHLIST>) {

      # Skip blank lines
      next if (/^\n$/);
      # Skip comments
      next if (/^#/);

      print "DEBUG: adding $_ to our list of devices \n" if ($DEBUG);

      push (@devices, $_);

   }

   close(SWITCHLIST);

   return 1;

}

############################################################################
# Subroutine prune_switches
#
# Purpose: prune list of switches to those that are support by this script
############################################################################
sub prune_switches {
   # Declare Local Variables
   my @list;
   my $xflag = 0;

   my $packet = Net::Ping->new('icmp');

   foreach $snmphost (@devices) {

      $xflag = 0;

      $snmphost =~ s/\n//g;

      if (!($packet->ping($snmphost))) {
         next;
      } #end if

      if (&grab_snmpsystem == 99) {
         next;
      } #end if

      foreach my $sysobject (@sysObjectIDs) {
         if ($sysObjectID eq $sysobject) {
            push(@list, $snmphost);
            print "DEBUG: $snmphost ($sysObjectID) is supported pushing to list...\n" if ($DEBUG);
            $xflag = 1;
            next;
         } #endif

      } #end foreach

      if ($xflag == 0) {
         print "DEBUG: $snmphost ($sysObjectID) is NOT supported!!!! ******** \n" if ($DEBUG);
      }

   } #end foreach

   @devices = @list;

   return 1;

} # end sub load_switches
############################################################################

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

   #print "DEBUG: snmphost = $snmphost and community = $community\n" if ($DEBUG);

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

   return 1;

}; #end sub grab_snmpsystem ########################################



########################################################################
# Figure out the current time
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
}

########################################################################
# Read in the data file
########################################################################
sub read_data {

  # Declare the Local Variables
  my $count;
  my $cTime;
  my $iTime;
  my $name;
  my $ifnum;
  my $card;
  my $port;
  my $vTime;

  # Open data file
  open DATA, "$portDataFile" or die "Can't open $dataFile: $!\n";

  # Walk through data file
  while (<DATA>) {

    # Skip blank lines
    next if (/^\n$/);

    # Skip comments
    next if (/^#/);

    # Read a line of data, throw away iTime using 'space' as the delimeter
    ($name, $ifnum, $card, $port, $count, $cTime, $vTime) = split(' ');

    # Build data structure in order to recall the data in the furture
    $disk{"$name $ifnum $card $port"} = "$name $ifnum $card $port $count $cTime $vTime";

  }

  close DATA;

  return 1;
}
########################################################################
# Write new data file
########################################################################
sub write_data {

  # Declare the Local Variables
  my $cTime;
  my $count;
  my $iTime;
  my $key;
  my $name;
  my $now;
  my $ifnum;
  my $port;
  my $slot;
  my $vTime;

  # Find time
  $now = time;

  # Open data file
  open DATA, ">$portDataFile";

  # Output all the following text 
print DATA <<EOF;
# Title:           Ethernet Switch Port Inventory Data
#
# Institution:     Your company here
#
# Date of Report:  $date at $time
#
# Description:     This report lists the most recent time at which data
#                  was received on a port in each of the monitored
#                  ethernet swiches.
#
# Definitions:     Switch       -- Name of the monitored switch
#                  IfNum        -- Interface number
#                  Card         -- Card slot in switch chassis
#                  Port         -- Port number on switch card(unit)
#                  InCount      -- Most recent reading of ifInOctets:
#                  Changed      -- Most recent time at which a change
#                                  in the ifInOctet count was noted
#                  Verified     -- Most recent time at which the octet
#                                  count was checked
#                  Idle         -- Time since last activity
#
#Switch                                     ifNum Card Port InCount  Changed     Verfied     Idle
#------------------------------------------ ----- ---- ---- -------- ----------- ----------- -------------
EOF

  # Loop through the hash splitting the stored data into individual variables
  for my $triple (sort byKeys (keys %data)) {
    ($name, $ifnum, $slot, $port, $count, $cTime, $vTime) = split(' ', $data{$triple});

    # If we haven't talked with this port for more than $goneDays, then
    # don't write information about the port to the data file
#    next unless ( $vTime > ($now - $goneDays*24*60*60) );

    # Calculate idle time
    $iTime = &calc_idle_time($cTime, $vTime);

    # Output special formatted text for report
    printf DATA "%-43s  %4s %4s %4s   %6s %11s %11s  %13s\n",
                 $name, $ifnum, $slot, $port, $count, $cTime, $vTime, $iTime;

  }
  
  # Close the file
  close DATA;

  return 1;
}


########################################################################
# Compare the current counts to those from the data file & update %disk
# After this routine runs, the information we've gathered and stored in
# %live has been merged into %disk
########################################################################
sub compare_counts {

  # Declare the Local Variables
  my $currCount;          # ifInOctets gathered live
  my $diskCount;          # ifInOctets from disk file
  my $diskCTime;          # Last time ifInOctets changed
  my $diskVTime;          # Last time we asked device if ifInOctets
                          # had changed
  my $flag;
  my $name;               # Host from data file
  my $ifnum;              # Index from data file
  my $port;               # Port from data file
  my $slot;               # Slot from data file

  # Walk through %live comparing the octet counter we just acquired with
  # the counter stored on disk
  for my $triple (keys %live) {

    # Get the current data
    $currCount = $live{$triple};

    # Grab an entry from the disk file
    if (exists $disk{$triple}) {
      ($name, $ifnum, $slot, $port, $diskCount, $diskCTime, $diskVTime) =
                                    split (' ', $disk{$triple});
    }

    # Compare
    if (! exists $disk{$triple} ) {             # New port
      $flag = "new";
      $data{$triple} = "$triple $currCount $currTime $currTime";
    }
    elsif ($currCount == $diskCount) {          # No change in byte count
      $flag = "no change";
      $data{$triple} = "$triple $diskCount $diskCTime $currTime";
    }
    else  {                                     # Recent activity
      $flag = "activity";
      $data{$triple} = "$triple $currCount $currTime $currTime";
    }

    # Debug info
    if ($DEBUG) {
      if    ($flag eq "new") {
        print "DEBUG: *NewPort* data{$triple} = $triple $currCount $currTime $currTime\n";
      }
      elsif ($flag eq "no change") {
        print "DEBUG: *NoDelta*  data{$triple} = $triple $diskCount $diskCTime $currTime\n";
      }
      elsif ($flag eq "activity") {
        print "DEBUG: *Change*  data{$triple} = $triple $currCount $currTime $currTime\n";
      }
    }
  }

# Debug info
#  if ($DEBUG) {
#    print "DEBUG: Dumping \%data\n";
#    for my $triple (sort keys %data) {
#      print "DEBUG: dumping data{$triple} = $data{$triple}\n";
#    }
#    print "\n";
#  }

  return 1;
}

###########################################################
# Figure out keys
###########################################################
#sub byKeys {
#  my ($a_host, $a_slot, $a_port, $b_host, $b_slot, $b_port);

#  ($a_host, $a_slot, $a_port) = split (' ', $a);
#  ($b_host, $b_slot, $b_port) = split (' ', $b);

#  if    (($a_host cmp $b_host) != 0)     { $a_host cmp $b_host; }
#  elsif (($a_slot <=> $b_slot) != 0)     { $a_slot <=> $b_slot; }
#  elsif (($a_port <=> $b_port) != 0)     { $a_port <=> $b_port; }

#}

########################################################################
# Subroutine byKeys
#
# Purpose: sort the DNS domain names from the Hash Array for output
########################################################################
sub byKeys {
   # Declare Local Variables
   my ($a_fqdn, $a_host, $a_domain1, $a_domain2, $a_domain3, $a_slot, $a_port);
   my ($b_fqdn, $b_host, $b_domain1, $b_domain2, $b_domain3, $b_slot, $b_port);

   ($a_fqdn, $a_slot, $a_port) = split (' ', $a);
   ($b_fqdn, $b_slot, $b_port) = split (' ', $b);

   ($a_host, $a_domain1, $a_domain2, $a_domain3) = split (/\./, $a_fqdn);
   ($b_host, $b_domain1, $b_domain2, $b_domain3) = split (/\./, $b_fqdn);

   #print "DEBUG: $a_host, $a_domain1, $a_domain2, $a_domain3\n" if ($DEBUG);
   #print "DEBUG: $b_host, $b_domain1, $b_domain2, $b_domain3\n" if ($DEBUG);
   #print "DEBUG: a = $a\n" if ($DEBUG);
   #print "DEBUG: b = $b\n" if ($DEBUG);

   if    (($a_domain3 cmp $b_domain3) != 0)  { $a_domain3 cmp $b_domain3; }
   elsif (($a_domain2 cmp $b_domain2) != 0)  { $a_domain2 cmp $b_domain2; }
   elsif (($a_domain1 cmp $b_domain1) != 0)  { $a_domain1 cmp $b_domain1; }
   elsif (($a_host cmp $b_host) != 0)        { $a_host cmp $b_host; }
   elsif (($a_host cmp $b_host) != 0)        { $a_host cmp $b_host; }
   elsif (($a_slot <=> $b_slot) != 0)        { $a_slot <=> $b_slot; }
   elsif (($a_port <=> $b_port) != 0)        { $a_port <=> $b_port; }

} #end sub bykeys


########################################################################
# Subroutine byKeys_dns
#
# Purpose: sort the DNS domain names from the Hash Array for output
########################################################################
sub byKeys_dns {
   # Declare Local Variables
   my ($a_fqdn, $a_host, $a_domain1, $a_domain2, $a_domain3, $a_slot, $a_port);
   my ($b_fqdn, $b_host, $b_domain1, $b_domain2, $b_domain3, $b_slot, $b_port);

   #($a_fqdn, $a_slot, $a_port) = split (' ', $a);
   #($b_fqdn, $b_slot, $b_port) = split (' ', $b);

   ($a_host, $a_domain1, $a_domain2, $a_domain3) = split (/\./, $a);
   ($b_host, $b_domain1, $b_domain2, $b_domain3) = split (/\./, $b);

   #print "DEBUG: $a_host, $a_domain1, $a_domain2, $a_domain3\n" if ($DEBUG);
   #print "DEBUG: $b_host, $b_domain1, $b_domain2, $b_domain3\n" if ($DEBUG);
   #print "DEBUG: a = $a\n" if ($DEBUG);
   #print "DEBUG: b = $b\n" if ($DEBUG);

   if    (($a_domain3 cmp $b_domain3) != 0)  { $a_domain3 cmp $b_domain3; }
   elsif (($a_domain2 cmp $b_domain2) != 0)  { $a_domain2 cmp $b_domain2; }
   elsif (($a_domain1 cmp $b_domain1) != 0)  { $a_domain1 cmp $b_domain1; }
   elsif (($a_host cmp $b_host) != 0)        { $a_host cmp $b_host; }

} #end sub bykeys






###########################################################
# Calculate idle time
###########################################################
sub calc_idle_time {
  my $cTime = shift;          # ChangeTime:  Time at which octet count
                              # last changed
  my $vTime = shift;          # VerifyTime:  Time at which we last
                              # talked with this port
  my $iTime;                  # IdleTime:  for how long has this port
                              # sat idle, expressed as
                              # days:hours:minutes:seconds

  my ($iDays, $iHours, $iMins, $iSecs);

  # Do the math
  if ($cTime != 0) {
      $iTime = $vTime - $cTime;

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

  }
  else {
      $iSecs  = 99;
      $iMins  = 99;
      $iHours = 99;
      $iDays  = 999;
  }

  # Put it all together
  $iTime = "$iDays:$iHours:$iMins:$iSecs";

  return $iTime;
}
###########################################################

###########################################################
# Count ports in various way
###########################################################
sub count_ports {
  my $count;
  my $cTime;
  my $name;
  my $ifnum;
  my $port;
  my $slot;
  my $vTime;

  # Initialize variables (only relevant in debugging cases)
  $activePorts = 0;
  $totalPorts = 0;

  # Walk through data and count ports
  for my $triple (sort keys %data) {

    # Grab next entry
    ($name, $ifnum, $slot, $port, $count, $cTime, $vTime) = split(' ', $data{$triple});

    # Initialize variables
    unless (exists $totalPorts{$name})  { $totalPorts{$name}  = 0 }
    unless (exists $activePorts{$name}) { $activePorts{$name} = 0 }
    unless (exists $idlePorts{$name}) { $idlePorts{$name} = 0 }

    # Increment totalPorts
    $totalPorts{$name}++;
    $totalPorts++;

    # Count idle ports.  For ports which have never seen traffic,
    # CatOS and IOS devices set ifInOctets to 0, whereas CatIOS devices
    # set this counter to 64.
    if ($count == 0 or $count == 64 or $cTime < $idleTime) {
      $idlePorts{$name}++;
      $idlePorts++;
    }
    # Count active ports
    else {
      $activePorts{$name}++;
      $activePorts++;
    }

  }

  # Debug info
  if ($DEBUG) {
    for my $name (sort keys %totalPorts) {
      print "For $name: $activePorts{$name} $totalPorts{$name}\n";
    }
    print "activePorts = $activePorts; totalPorts = $totalPorts\n";
  }

  return 1;

}
###########################################################

###########################################################
# Subroutine to retreive SNMP data and store it
###########################################################
sub getsnmpdata {
   my $packet = Net::Ping->new('icmp');

   foreach $snmphost (@devices) {

      $snmphost =~ s/\n//g;

      if ($packet->ping($snmphost)) {

         my $sess = new SNMP::Session (
                        DestHost  => $snmphost,
                        Community => $community,
                        Version   => SNMPVER );

         my $total = $sess->get($iftotal);

         # We need to know the sysObjectID of the switch we are about
         # to walk so we can make decisions. We also need it to be global
         # since we will call other subroutines which also need to know
         # the sysObjectID
         $sysObjectID = $sess->get('sysObjectID.0');

         # Some products behave differently based on software release, so
         # let's try and determine what software release this switch is running
         &check_software;

         if (! defined $total) {
            print "ERROR: snmp query against $snmphost for number of intfs failed!!! skipping!!!\n";
            next;
         }

         ##$sysObjectID =~ s/.1.3.6.1.4.1/enterprises/;

         print "DEBUG: total interfaces for $snmphost equal $total\n" if ($DEBUG);
         print "DEBUG: system object ID = $sysObjectID\n" if ($DEBUG);

         my $vars = new SNMP::VarList(
                        ['ifIndex', 0],
                        ['ifInOctets', 0],
                        ['ifType',0]
                        );

         while (1) {

            @vals = $sess->getnext($vars);

#         print  $vars->[0]->tag, "\n" ;
#         print  $vars->[1]->tag, "\n" ;
#         print  $vars->[2]->tag, "\n" ;
#         print  $vars->[3]->tag, "\n" ;

            my $ifIndex = $vals[0];
            my $ifInOctets = $vals[1];
            my $ifType = $vals[2];

            # Let's skip the interfaces that aren't physical or real
            next if ( ($ifType eq "propMultiplexor") ||
                      ($ifType eq "l2vlan") ||
                      ($ifType eq "l3ipvlan") ||
                      ($ifType eq "other") ||
                      ($ifType eq "propVirtual") );

            last unless ($vars->[0]->tag eq 'ifIndex');
            
            #print "DEBUG: ifIndex $ifIndex reports ifInOctets = $ifInOctets \n" if ($DEBUG);

            if ( ($sysObjectID eq "rcA8610") ||
                 ($sysObjectID eq "rcA8606") ||		# enterprises.2272.30
                 ($sysObjectID eq "rcA1648") ) {	# enterprises.2272.43
               # Ethernet Routing Switch 8610/8606/1648
               $card = int($ifIndex / 64);
               $port = ( $ifIndex - ($card * 64) + 1 );
               #print "DEBUG: Passport 8600 Interface card=int($ifIndex/64)~$card port=($ifIndex-($card*64)+1)~$port \n" if ($DEBUG)
            } elsif ( ($sysObjectID eq "sreg-BayStack470-48T-ethSwitchNMM" ) ||
                      ($sysObjectID eq "sreg-BayStack470-24T-ethSwitchNMM" ) ||
                      ($sysObjectID eq "sreg-BayStack470-48T-PWR-ethSwitchNMM") ) {
               # BayStack 470 Switch Series
               $card = int($ifIndex / 64);
               $port = ( $ifIndex - ($card * 64) );
               $card++;
            } elsif ( ($sysObjectID eq "BPS2000-24T-ethSwitchNMM") ||
                      ($sysObjectID eq "sreg-BayStack460-24T-PWR-ethSwitchNMM") ||
                      ($sysObjectID eq "sreg-BayStack450-ethSwitchNMM" ) ||
                      ($sysObjectID eq "sreg-BayStack350-24T-ethSwitchNMM") ) {
               $card = int($ifIndex / 32);
               $port = ( $ifIndex - ($card * 32) );
               $card++;
            } elsif ($sysObjectID eq "sreg-BPS2000-24T-ethSwitchNMM" ) {
               # Business Policy Switch
               # sreg-BPS2000-24T-ethSwitchNMM  enterprises.45.3.40.1
               $card = 1;
               $port = $ifIndex;
            } elsif (($sysObjectID eq "sreg-SMB-BES-50-GE-24T-PWR") ||
                     ($sysObjectID eq "sreg-BayStack303-304-Sw" ) ||
                     ($sysObjectID eq "sreg-BayStack350-ethSwitchNMM" ) ||
                     ($sysObjectID eq "sreg-ERS-4548GT-PWR" )) {
               # sreg-BayStack303-304-Sw  enterprises.3.32.1
               # sreg-BayStack350-ethSwitchNMM  enterprises.3.30.1
               # sreg-ERS-4548GT-PWR
               # sreg-SMB-BES-50-GE-24T-PWR
               $card = 1;
               $port = $ifIndex;
            } elsif (($sysObjectID eq "sreg-BayStack5510-48T-ethSwitchNMM") || 
                     ($sysObjectID eq "sreg-BayStack5510-24T-ethSwitchNMM") ||
                     ($sysObjectID eq "sreg-BayStack5520-24T-PWR") ||
                     ($sysObjectID eq "sreg-BayStack5520-48T-PWR") ||
                     ($sysObjectID eq "sreg-EthernetRoutingSwitch5530-24TFD")) {
               if ( $sysSoftware =~ "6.1" ) {
                  ##$ifnum = (( $slot * 128 ) + $port ) - 128;   #### copied from ARP/FDB report script
                  $card = int ($ifIndex / 128);
                  $port = $ifIndex - ($card * 128);
                  $card++;
               } else {
                  ##$ifnum = (( $slot * 64 ) + $port ) - 64;     #### copied from ARP/FDB report script
                  $card = int($ifIndex / 64);
                  $port = $ifIndex - ($card * 64);
                  $card++;
               }
            } elsif (($sysObjectID eq "hpProLiant-p-GbE2-InterconnectSwitch" ) ||
                     ($sysObjectID eq "hpProLiant-p-GbE2c-InterconnectSwitch") ||
                     ($sysObjectID eq "hpProLiant-GbE2c-InterconnectSwitch")) {
               # HP GbE2/GbE2c Switch Blade hpProLiant-GbE2c-InterconnectSwitch
               $card = 1;
               if ($ifIndex > 255) { $port = ( $ifIndex - 256 ); }
               else { $port = $ifIndex; }
            } elsif (($sysObjectID eq "catalyst2924XLv") ||
                     ($sysObjectID eq "catalyst295048G") ||
                     ($sysObjectID eq "catalyst2924MXL") ||
                     ($sysObjectID eq "catalyst2950t24") ||
                     ($sysObjectID eq "catalyst295048G") ||
                     ($sysObjectID eq "catalyst295024G") ||
                     ($sysObjectID eq "catalyst6kMsfc2") ||
                     ($sysObjectID eq "catalyst295012") ){
               # Cisco
               $card = 1;
               $port = $ifIndex;
            } elsif (($sysObjectID eq "catalyst296024") ||
                     ($sysObjectID eq "catalyst356024PS") ||
                     ($sysObjectID eq "catalyst296048TT") ) {
               # Cisco 2960 / 3650 Switch
               $card = 1;
               if ($ifIndex > 10000) {$port = $ifIndex - 10000;} else { $port = $ifIndex;}
            } elsif (($sysObjectID eq "catalyst2960G48") ||
                     ($sysObjectID eq "catalyst356024PD") ||
                     ($sysObjectID eq "catalyst3560E24PD") ||
                     ($sysObjectID eq "catalyst3560G48TS") ||
                     ($sysObjectID eq "catalyst3560G24PS") ||
                     ($sysObjectID eq "catalyst3560G48PS") ) { 
               # Cisco 3560 Switches
               $card = 1;
               if ($ifIndex > 10000) { $port = $ifIndex - 10100;} else { $port = $ifIndex;}
            } elsif (($sysObjectID eq "catalyst37xxStack") ) {
               # Cisco 3750 Switches
               if ( ($ifIndex >= 10100) and ($ifIndex < 10200)) {
                  $card = 1;
                  $port = $ifIndex - 10100;
               } elsif ( ($ifIndex >= 10600) and ($ifIndex < 10700)) {
                  $card = 2;
                  $port = $ifIndex - 10600;
               }

            } else {
               print "ERROR: Unable to determine switch model for $snmphost ($sysObjectID)!!!! \n";
               next;
            }

            $live{"$snmphost $ifIndex $card $port"} = substr ($ifInOctets, -6, 6);

         } #end while loop for snmpgetnext

      } else { #end if packet-ping
         print "DEBUG: $snmphost did not respond to ICMP ping request\n";
      }
                       
   } #end foreach $snmphost

   return 1;

}
###########################################################

########################################################################
# Print summary
########################################################################
sub write_summary {
  my $active;                # Percent of active ports on this device
  my $idle;                  # Percent of idle ports on this device
  my $per = "%";             # Percent sign
  my $total;                 # Sum of activePorts and idlePorts on that host
  my $totalidle;
  my $totalidlepercent;

  # Open report file
  unless (open SUMMARY, ">$summaryFile") {
     &print_it ("Cannot open $summaryFile: $!");
     return 0;
  }

  $totalidle = ($totalPorts - $activePorts);

  my $temptotalidlepercent = (($totalidle / $totalPorts) *100);

  # Round number to 3 digits after decimal point
  #$rounded = sprintf("%.3f", $number);
  $totalidlepercent = sprintf( "%.3f", $temptotalidlepercent);

  print SUMMARY <<EOF;
# Title:           Switch Port Utilization Summary
#
# Institution:     Your Company
#
# Date of Report:  $date at $time
#
# Description:     This report lists switch ports which have not
#                  received data for more than $idleDays days. Or
#                  ports which indicate that they have never recieved
#                  any data since their last reboot.
#
# Total:           $totalPorts
# Active:          $activePorts
# Idle:	           $totalidle ($totalidlepercent %)
#
#
# Switch                                        Total  Active   Idle   % Active   % Idle
# -------------------------------------------   -----  ------   ----   --------   ------
EOF

  for my $host (sort byKeys_dns (keys %activePorts)) {
    print "host = $host activePorts = $activePorts{$host} idlePorts = $idlePorts{$host}\n" if ($DEBUG); 
    $total = $activePorts{$host} + $idlePorts{$host};
    $active = int (($activePorts{$host}/$totalPorts{$host} * 100) + .5);
    $idle   = int (($idlePorts{$host}/$totalPorts{$host}   * 100) + .5);
    printf SUMMARY "%-43s   %5s  %6s   %4s   %7s%1s   %5s%1s\n",
        $host, $total, $activePorts{$host}, $idlePorts{$host},
        $active, $per, $idle, $per;
  }
  close SUMMARY;

 return 1;
}


########################################################################
# Print report
########################################################################
sub write_report {
  my $cHost = "";         # 'Current host':  host whose entries in the
                          # data file we are examining
  my $cSlot = "";         # 'Current slot':  slot on $cHost whose entries
                          # in the data file we are examining
  my $first = 1;          # Hack for identifying first host in report
  my $fSlot = "";         # 'First Slot':  first slot in current host
                          # (generally, though not always, equal to "1")
  my $idle = "";          # String returned from &packIt; contains
                          # the contents of @cHosts in 'packed' format
  my @iPorts;             # 'Idle Ports':  idle ports on $name
                          # Parameters from current line in data file
  my ($name, $ifnum, $slot, $port, $count, $cTime, $vTime, $iTime);
  my $totalidle;
  my $totalidlepercent;

  # Open report file
  unless (open REPORT, ">$reportFile") {
     &print_it ("Cannot open $reportFile: $!");
     return 0;
  }

  $totalidle = ($totalPorts - $activePorts);
  #$totalidlepercent = int( (($totalidle / $totalPorts) *100) +.5);
  my $temptotalidlepercent = (($totalidle / $totalPorts) *100);

  # Round number to 3 digits after decimal point
  #$rounded = sprintf("%.3f", $number);
  $totalidlepercent = sprintf( "%.3f", $temptotalidlepercent);


  print REPORT <<EOF;
# Title:           Idle Switch Ports
#
# Institution:     Your Company
#
# Date of Report:  $date at $time
#
# Description:     This report lists switch ports which have not
#                  received data for more than $idleDays days
#
# Total:           $totalPorts
# Active:          $activePorts
# Idle:            $totalidle ($totalidlepercent %)
#
# Switch                                     Card  Idle Ports
# -----------------------------------------  ----  ---------------------------------------------
EOF

  # Walk through data file, printing idle ports on hosts as we go
  for my $triple (sort byKeys (keys %data)) {

    # Grab next line
    ($name, $ifnum, $slot, $port, $count, $cTime, $vTime) = split(' ', $data{$triple});

    # If this is the first port on the first host of the report, don't worry
    # about previous hosts (there weren't any) and just examine the current
    # port's cTime and vTime
    if ($first == 1) {
      $first = 0;
      $cHost = $name;
      $fSlot = $cSlot = $slot;
      if ((&oldEnough ($cTime, $vTime)) or ($count == 0)) { push @iPorts, $port }
    }                         # End of processing first port on first host

    # New host:  print the previous slot's info
    elsif ($cHost ne $name) {

      # Send &packIt the list of idle ports
      $idle = &packIt(@iPorts);

 
      # If this is the first (and only) line for $cHost, print $cHost
      # (highly likely that $cSlot equals "1" in this case)
      if ($cSlot == $fSlot) {
        printf REPORT "%-43s   %-4s  %-40s\n", $cHost, $cSlot, $idle;
      }
      else            {                # don't print $cHost
        printf REPORT "                                              %-4s  %-40s\n", $cSlot, $idle;
      }
      printf REPORT "\n";

      # OK, now that we've printed the last slot on the previous host,
      # let's process the first port on the first slot of this new host
      $cHost = $name;
      $fSlot = $cSlot = $slot;
      undef @iPorts;
      if ((&oldEnough ($cTime, $vTime)) or ($count == 0)) { push @iPorts, $port }

    }                         # End of processing new host

    # New slot on same host:  print previous slot's info
    elsif ($cSlot ne $slot) {

      # Send &packIt the list of idle ports
      $idle = &packIt(@iPorts);

      # If this is the first line for $cHost, print $cHost; otherwise, don't
      if ($cSlot == $fSlot ) {
        printf REPORT "%-43s   %-4s  %-40s\n", $cHost, $cSlot, $idle;
      }
      else            {
        printf REPORT "                                              %-4s  %-40s\n", $cSlot, $idle;
      }

      # OK, now that we've printed the previous slot, let's process this
      # first port on this new slot
      $cSlot = $slot;
      undef @iPorts;
      if ((&oldEnough ($cTime, $vTime)) or ($count == 0)) { push @iPorts, $port }

    }                         # End of processing new slot on same host


    # Same host, same slot:  if the port is idle, add it to @iPorts
    elsif ((&oldEnough ($cTime, $vTime)) or ($count == 0)) {push @iPorts, $port }

  }                           # End of loop


  # Print the last slot on the last host

    # Send &packIt the list of idle ports
    $idle = &packIt(@iPorts);

    # If this is the first line for $cHost, print $cHost; otherwise, don't
    if ($cSlot == $fSlot) {
      printf REPORT "%-43s   %-4s  %-40s\n", $cHost, $cSlot, $idle;
    }
    else            {
      printf REPORT "                                             %-4s  %-40s\n", $cSlot, $idle;
    }

  close REPORT;

  return 1;

}
########################################################################
# Pack data
########################################################################
sub packIt {
  my @ports = @_;
  my ($i, $iN, $iZ, @zero, @new, $string);

  # If we have work to do, do it; else return white space
  if (@ports) {

    # Convert interior numbers in a consecutive run to 0
    for ($i = 0; $i < $#ports; ++$i) {
      if    ($ports[$i-1] != $ports[$i]-1) { $zero[$i] = $ports[$i] }
      elsif ($ports[$i+1] != $ports[$i]+1) { $zero[$i] = $ports[$i] }
      else                                 { $zero[$i] = 0          }
    }

    $zero[0]       = $ports[0];
    $zero[$#ports] = $ports[$#ports];

    # Eliminate consecutive 0s
    $new[0] = $zero[0];
    for ($iZ = 1, $iN = 1; $iZ <= $#zero; ++$iZ, ++$iN) {

      if    ($zero[$iZ] != 0)           { $new[$iN] = $zero[$iZ] }
      elsif ($zero[$iZ] != $new[$iN-1]) { $new[$iN] = $zero[$iZ] }
      else                              { --$iN                  }
    }

    # Convert single 0s to dashes
    $string = $new[0];
    for ($i = 1; $i <= $#new; ++$i) {
      if (($new[$i] != 0) and ($new[$i-1] != 0)) { $string .= ","        }
      if ( $new[$i] == 0)                        { $string .= "-"        }
      else                                       { $string .= "$new[$i]" }
    }
  }

  else { $string = " " }

  return $string;
}

########################################################################
# Am I old enough?  usage:  &oldEnough ($cTime, $vTime)
########################################################################
sub oldEnough {
  my ($a, $b) = @_;

  if ($b < $currTime)           { 0 }          # Stale data
  elsif ($a == 0)               { 1 }          # Never used
  elsif ($a > $idleTime)        { 0 }          # Not old enough
  else                          { 1 }          # Old enough

}

############################################################################
# Subroutine check_software
#
# Purpose: perform SNMP query against device and return software version
############################################################################
sub check_software {

   my $check_sess = new SNMP::Session ( DestHost  => $snmphost,
                                        Community => $community,
                                        Version   => SNMPVER );

   print "DEBUG: check_software $snmphost with $sysObjectID\n" if ($DEBUG);

   foreach my $supported (@AvayaERSSwtichStacks) {
      ##print "DEBUG: check_software $sysObjectID == $supported\n" if ($DEBUG);

      if ($sysObjectID eq $supported) {

         # Let's get the software release from the Nortel/Avaya switches
         $sysSoftware =  $check_sess->get("s5AgInfoVer.0");

         if ( $check_sess->{ErrorStr} ) {
            print "ERROR: sess->{Errocheck_software rStr} = $check_sess->{ErrorStr}\n";
         }

         print "DEBUG: check_software $snmphost sysSoftware = $sysSoftware\n" if ($DEBUG);

      } #endif
   } #end foreach

   return 1;

};
#end sub check_software ########################################


