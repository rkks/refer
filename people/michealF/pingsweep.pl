#!/usr/bin/perl
#
# Filename: /usr/local/etc/pingsweep.pl
#
# Purpose:  ping a list of network electronics and report (email) a
#           list of devices that does not respond.
#
# Author:   Michael McNamara
#
# Language: Perl
#
# Date:     May 6, 2003
#
# Changes:
#
#    Sep 20, 2007: cleanup code and comments in order to publish code to web
#    Dec 30, 2005: cleaned up DEBUG and logging statements for troubleshooting
#    Mar 18, 2005: added file logging for troubleshooting and monitoring
#    Feb 19, 2005: added threshold logic to prevent flapping notifications
#    Feb 17, 2005: fixed logic regarding alert notification and ping retry
#    Jan 17, 2005: added code that will retry ping if # devices alarming is < 10 devices
#   July 14, 2004: script will now report when devices are once again reachable
#    Jan  5, 2004: record downtime for monthly computation of downtime %
#    Jan  2, 2004: changed alert notifications so that device must be
#                  down for at least 60 seconds before email notification
#                  Also added 15 minute alert email notification
#                  Notifications now: > 1 min and < 5 min
#                                     at 15 min
#                                     at 60 min
#    Dec 30, 2003: changed email notifications to HTML based messages
#
# Notes:
#        Over the years I've built up this Perl script from a simple shell
#        that would just call FPING to a all encompassing tool which would
#        still perform its basic function yet go well beyond its original
#        design. What does it have now that it didn't have then you ask?
#        Well lets name a few; debug output, comments are now allowed in
#        the input files, ping retry, flapping notification thresholds, etc.
#        And it currently has timing logic to initiate both email 
#        notification and pager notification. It now has threshold logic
#        to prevent those few occasions where I would find a few hunderd 
#        alert messages in my inbox because of some flapping condition. It
#        also has the ability to record the outages so that a downtime
#        report can be generated from that data (reportdowntime.pl).
#
#        This script requires fping; http://fping.sourceforge.net/
#
#        This script was written and designed to run on a Fedora Core Linux
#        server. It can probably be adapted to run on a Windows server.
#

# Load Modules
use strict;
use warnings;
use Fcntl qw(:DEFAULT :flock);

# Declare Constants
use constant DEBUG      => 1;           # DEBUG settings
use constant CONSOLE    => 0;           # CONSOLE DEBUG settings

# Declare Global Variables
my $result;
my $sdate;
my $date;
my $time;
my $currentTime;
my @devices;

my $retry = 10;	# Number of seconds to wait before retrying ping
my $pause = 7;  # Number of seconds to pause at the startup of the script

my %disk;	# Array of Devices that were previously DOWN
my %data;	# Array of Devices that are currently DOWN (after processing)
my %live;	# Array of Devices that was recorded as DOWN from FPING
my %notify;	# Array of Devices that will be included in notifications
my %thres;	# Array of Devices for threshold checking

my $FPING = "/usr/local/etc/fping";	# Location of FPING exec

my $MAILTO;			# Who should recieve the email alerts
my $MAILFROM;			# Who they should appear as coming from
my $MAILSUBJECT;		# The subject of the email alerts
my $PAGERTO;			# The email pager addresses of those that should be paged
my $NOTIFY;			# Flag/count variable for email notification
my $PAGERNOTIFY = 0;		# Flag/count variable for pager notification
my $SOMETHINGTODO = 0;		# Flag/count variable for alert logic
my $RECOVER = 0;		# Flag/count variable for recovery notification

my $program = "pingsweep.pl";
my $version = "v1.2";
my $author = "Michael McNamara";
my $purpose = "This Perl script is designed to poll the network electronics and report any failures.";
my $usage = "Usage: pingsweep.pl \[debug\]\n";

# Email Addresses and Subject Lines
$MAILTO = '';
$MAILFROM = '';
$MAILSUBJECT = 'ALERT: Urgent Network Status Report';
$PAGERTO = '';

# Data File Paths and Locations
my $electronics		= "/usr/local/etc/pingsweep.txt";
my $datafile		= "/usr/local/etc/pingsweep.dat";
my $flagfile		= "/tmp/pingsweep.flg";
my $recordfile		= "/usr/local/etc/pingsweeprecord.dat";
my $thresholdfile	= "/usr/local/etc/pingsweeprecord.dat";
my $logfile		= "/usr/local/etc/logs/pingsweep.log";
my $lockfile		= "/tmp/pingsweep.tmp";
my $templock		= "/tmp/pingsweep.tmp.$$";

# Notification Intervals in seconds
our $firstalert      = 15*60;		# Time value for first alert notification
our $secondalert     = 60*60;		# Time value for second alert notification
our $minalert        = 5*50;		# Time value for minimum alert notification
our $largealert      = 10;		# Number of alert devices to send immediate
our $thresholdTime   = 120*60;		# Time (seconds) value for threshold checking
our $thresholdEvents = 3;		# Threshold value for number of events 
our $NOW = localtime;			# Local Date and Time 

###########################################################################

# Let's trap and SIG interrupts and remove lockfiles and tempfiles
   $SIG{INT} = $SIG{TERM} =
           sub {   unlink ${main::Cleanfile}
                       if defined $main::Cleanfile;
                   unlink ${main::Cleanfile2}
                       if defined $main::Cleanfile2;
                   unlink ${main::Cleanfile3}
                       if defined $main::Cleanfile3;
                   warn "$NOW: ERROR: Bailout after SIG $_[0]\n";
                   exit 1;
                };
  $SIG{HUP} = sub {
                   unlink ${main::Cleanfile}
                       if defined $main::Cleanfile;
                   unlink ${main::Cleanfile2}
                       if defined $main::Cleanfile2;
                   unlink ${main::Cleanfile3}
                       if defined $main::Cleanfile3;
                   die "$NOW: ERROR: Bailout after SIG $_[0]\n";
                };

END {
    local($?, $!);
    unlink ${main::Cleanfile} if defined $main::Cleanfile;
    unlink ${main::Cleanfile2} if defined $main::Cleanfile2;
}


###########################################################################
#  B E G I N   M A I N
###########################################################################

# Initialize program environment
&initialize;

# Load threshold data and identify problem devices
&load_thres;

# Ping devices and get list of devices not responding
&get_data;

# Load devices that failed to respond previously
&load_data;

# Compare the list of devices responding against the old list
&comp_data;

# Check to see if email alerts should be sent
&check_alert;

# Store list of devices not responding for later use
&write_data;

# Issue email for list of devices not responding or now responding
&alert_notify;

# Issue pager email alert if more than 10 devices for 5 minutes
&alert_pager;

# Store list of devices that are now responding with their total 
# accumulated downtime for later reporting with reportdowntime.pl
&record_downtime;

# Remove the flagfile after everything has completed
&finishup;

exit 0;

###########################################################################
#  E N D   M A I N
###########################################################################


###########################################################################
# Subroutine initialize
#
# Purpose: perform all the initialization steps and procedures
###########################################################################
sub initialize {

   # Declare Local Variables

   # Inialize time and date
   $sdate = localtime;
   ($date, $time) = &get_time;
   $currentTime = time;

   &lockit($lockfile,$templock);

   ## Let's open the logfile and place a lock on the file
   open(LOGFILE, ">>$logfile");
   flock(LOGFILE, LOCK_EX) or die "$program script unable to lock logfile $logfile\n";

   logit("DEBUG: #################################################################", 1);
   logit("DEBUG: (initialize) $program $version starting up...", 1);
   logit("DEBUG: (initialize) logfile $logfile is open and flock complete", 1);
   logit("DEBUG: (initialize) creating flagfile $flagfile", 1);
   
   logit("DEBUG: (initialize) sleeping $pause seconds",1) if (DEBUG);

   # sleep 1 seconds to offset other test scripts
   sleep $pause;

   return;

} #end sub


###########################################################################
# Subroutine finishup
#
# Purpose: wrap up the program and close any open files
###########################################################################
sub finishup {

   logit("DEBUG: (finishup) all finished let's remove the flagfile.",1) if (DEBUG);

   ## Remove the flagfile after everything has completed
   ##`/bin/rm $flagfile`;
   #unlink $flagfile;
   close LOCK; unlink ($templock, $lockfile);

   logit("DEBUG: (finishup) closing logfile $logfile",1) if (DEBUG);
   # Close the log data file
   close(LOGFILE);

   # Copy the list of down devices for the webserver to access
   `cp -f /usr/local/etc/pingsweep.dat /var/www/html/pingsweep.dat`;  

   return;
}


###########################################################################
# Subroutine get_data
#
# Purpose: ping a list of devices and record those that don't respond
###########################################################################
sub get_data {
   # Declare Local Variables
   my $name;
   my $execstr;
   my $result2;
   my $numdevices;	# Number of elements in @devices array

   logit("DEBUG: (get_data) shelling out to exec system call to FPING",1) if (DEBUG);

   # Use FPING to ping all the electronics and store the list of devices
   # that doesn't respond into the string $result
   $result = `$FPING -u -f $electronics`;

   # If there were some devices that didn't respond let's go to work
   if ($result) { 

      $SOMETHINGTODO = 1; 	# Set this flag for later processing

      chomp ($result);		# remove the CR/LF

      logit("DEBUG: (get_data) some devices failed to respond to our ping",1) if (DEBUG);

      
      # Lets take the results and load them into an array
      @devices = split (/\n/, $result);

      # Lets count the number of elements in the array
      $numdevices = @devices;

      # If there are less than 10 devices not responding lets go ahead and
      # try to PING them again just to make sure they are really down. If
      # there are more than 10 devices we can assume that this is a larger
      # problem and that we shouldn't overload the system trying to re-PING
      # too many devices in too short a timeframe else we'll go outside our
      # 60 second window.
      if ($numdevices < 10) {

         # Lets take that list of devices and ping them again to confirm
         $execstr = "$FPING -u ";
      
         foreach $name (@devices) {
            $execstr = $execstr . "$name ";
         }
         logit("DEBUG: (get_data) some devices failed to responsd, retrying...",1) if (DEBUG);
         logit("DEBUG: (get_data) here's the execstr $execstr",1) if (DEBUG);
         logit("DEBUG: (get_data) sleeping the retry interval of $retry seconds",1) if (DEBUG);

         # Lets stop and catch our breath for a few seconds before trying again
         sleep $retry;

         # Lets ping those devices that originally failed again
         $result2 = `$execstr`;

         # If there were any results lets store and evaluate them
         if ($result2) {

            $SOMETHINGTODO = 1; 	# Set this flag for later processing
	    	
 	    chomp ($result2);		# remove CR/LF

 	    # Lets take the results and load them into an array
            @devices = split(/\n/, $result2);

            logit("DEBUG: (get_data) some devices failed to respond a SECOND time",1) if (DEBUG);

            foreach $name (@devices) {
               $name =~ s/\/n//g;			# remove the CR/LF
               $live{$name} = $currentTime;	# store the current time for each device
               logit("DEBUG: (get_data) live{$name} = $live{$name}",1) if (DEBUG);
            } #end foreach

         } else {	# if there were no devices down the SECOND time around
	      
            $SOMETHINGTODO = 0;		# Set this flag for later processing
	 
            logit("DEBUG: (get_data) all devices responded the SECOND time around.",1) if (DEBUG);

         } #end if else($result2)

      } else {		# if number of devices down > 10 just process

           logit("DEBUG: (get_data) number of devices > 10 processing without retrying PING",1) if (DEBUG);

           foreach $name (@devices) {
              $name =~ s/\/n//g;		# remove the CR/LF
              $live{$name} = $currentTime;	# store the current time for each device
              logit("DEBUG: (get_data) live{$name} = $live{$name}",1) if (DEBUG);
           } #end foreach

      } # endif numdevices > 10

   } else { 		# if there were no devices down the FIRST time around

      # There were no devices that failed to respond so there's nothing to-do
      $SOMETHINGTODO = 0;

      logit("DEBUG: (get_data) there were no devices that failed to respond...",1) if (DEBUG);
   } #end if($result)

return 1;
}

##########################################################################
# Subroutine alert_notify
#
# Purpose: compose an HTML based email message which details the devices
#          that failed to response and also details those devices that
#          are now reponding (recovered).
##########################################################################
sub alert_notify {

   # Declare Local Varaibles
   my $name;
   my $oTime;
   my $lTime;
   my $dTime;
   my $alert;
   my $flag = 0;

   # If there was some device that either failed to ping or has recovered
   if ($NOTIFY | $RECOVER) {

      logit("DEBUG: (alert_notify) there is something todo",1) if (DEBUG);

      # Lets open a filehandle to sendmail for an email alert
      open(SENDMAIL, "| /usr/lib/sendmail $MAILTO") || die;

      # We need to make sure we properly format the mail message
      print(SENDMAIL "From: $MAILFROM\nTo: $MAILTO\nSubject: $MAILSUBJECT\n");
      print(SENDMAIL "MIME-Version: 1.0\n");
      print(SENDMAIL "Content-Type: text/html; charset=us-ascii\n\n");
      print(SENDMAIL "<FONT Face=Verdana SIZE=2>\n");

      print SENDMAIL <<EOF;
<img height="50" src="http://madmax/image6.gif" width="175" border="0" align="right"></p>
<p>
<B>Main Line Health Network Infrastructure Status Report</B><BR>\n
Date : $sdate<BR>
EOF

      # There was a device that failed to respond so we'll be alerting on it
      if ($NOTIFY) {

         logit("DEBUG: (alert_notify) within the down host section",1) if (DEBUG);

         print SENDMAIL "<p>\n";
         print SENDMAIL "The following devices failed to respond to an ICMP ping(s);<BR>\n";

         print SENDMAIL "<table border=0 width=100%>";
         print SENDMAIL "<tr>";
         print SENDMAIL "<td><FONT Face=Verdana SIZE=2><b>Device Hostname or IP Address</b></font></td>";
         print SENDMAIL "<td><FONT Face=Verdana SIZE=2><b>Time Down<br>DD:HH:MM:SS</b></font></td>";
         print SENDMAIL "</tr>";

        # processing in here for devices that failed
        for my $index (sort keys %notify) {
           ($name, $oTime, $lTime, $dTime, $alert) = split(' ', $notify{$index});

           # Calculate down time
           $dTime = &calc_down_time($oTime, $currentTime);
   
           print SENDMAIL "<tr>\n";
           print SENDMAIL "<td><FONT Face=Verdana SIZE=2>$name</font></td>\n";
           print SENDMAIL "<td><FONT Face=Verdana SIZE=2>$dTime</font></td>\n";
           print SENDMAIL "</tr>\n";
           logit("DEBUG: (alert_notify) Down Hostname = $name \tDownTime = $dTime",1) if (DEBUG);
        }

        print SENDMAIL "</table>\n";

      } #end if $NOTIFY

      # There was some device that recovered so we'll be alerting on it
      if ($RECOVER) {

         logit("DEBUG: (alert_notify) within the recovered host section",1) if (DEBUG);

         print SENDMAIL "<p>\n";
         print SENDMAIL "The following devices are <b>now</b> responding to ICMP ping(s);<BR>\n";
         print SENDMAIL "<table border=0 width=100%>\n";
         print SENDMAIL "<tr>\n";
         print SENDMAIL "<td><FONT Face=Verdana SIZE=2><b>Device Hostname or IP Address</b></font></td>\n";
         print SENDMAIL "<td><FONT Face=Verdana SIZE=2><b>Time Down<br>DD:HH:MM:SS</b></font></td>\n";
         print SENDMAIL "</tr>\n";

         # processing in here for devices that recovered
         for my $triple (sort keys %disk) {

            # If device is still down we shouldn't report it as recovered
            next if ( exists ($data{$triple}) );

            ($name, $oTime, $lTime, $dTime) = split(' ', $disk{$triple});

            # Threshold checking
            if ( exists( $thres{$name} ) && ($thres{$name} > $thresholdEvents)) {
	       logit("DEBUG: (alert_notify) threshold reached $name skipping recovery email.",1) if (DEBUG);
	       next;
	    }

            # Calculate down time
            $dTime = &calc_down_time($oTime, $currentTime);

            print SENDMAIL "<tr>\n";
            print SENDMAIL "<td><FONT Face=Verdana SIZE=2>$name</font></td>\n";
            print SENDMAIL "<td><FONT Face=Verdana SIZE=2>$dTime</font></td>\n";
            print SENDMAIL "</tr>\n";
            logit("DEBUG: (alert_noitfy) Up Hostname = $name \tDownTime = $dTime\n",1) if (DEBUG);

         } #end for my $triple

       print SENDMAIL "</table>\n";

      } #end if $RECOVER

      print SENDMAIL <<EOF;
<P>
<FONT Face=Verdana SIZE=2>Notes: this message is now being sent in HTML format.
</font>
<P>
<IMG SRC=http://ziggy.mdc./powered_by_perl.gif><BR>\n
<IMG SRC=http://ziggy.mdc./powered_by_redhat.gif><BR>\n

EOF

      close(SENDMAIL) || die;

   } #end if ($NOTIFY | $RECOVER)
   else {
      logit("DEBUG: (alert_notify) there is nothing to notify",1) if (DEBUG);
   }

return 1;
} #end sub alert_notify

########################################################################
# Subroutine get_time
#
# Purpose: calculate the time
########################################################################
sub get_time {
   # Declare Local Variables
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

########################################################################
# Subroutine load_data
#
# Purpose: load from file the list of devices that were previously down
########################################################################
sub load_data {

   # Declare Local Variables
   my $oTime;		# Original timestamp when device went down
   my $lTime;		# Last timestamp when device was checked
   my $rTime;		# Recovery timestap when device recovered
   my $dTime;		# Amount of time the device has been down
   my $name;		# FQDN of the device being checked
   my $alert;		# Number of Pager alerts sent

   # Open data file
   open DATA, "$datafile" or die "Can't open $datafile: $!\n";

   logit("DEBUG: (load_data) starting to load hash \%disk",1) if (DEBUG);

   # Walk through data file
   while (<DATA>) {

      # Skip blank lines
      next if (/^\n$/);
      # Skip comments
      next if (/^#/);

      # Read a line of data, throw away iTime
      ($name, $oTime, $lTime, $dTime, $alert) = split(' ');

      # Build data structure
      $disk{$name} = "$name $oTime $lTime $dTime $alert";

      logit("DEBUG: (load_data) reading disk{$name} = $name $oTime $lTime $dTime $alert",1) if (DEBUG);

   } #end while

   close DATA;

   return;
} #end sub load_data

########################################################################
# Subroutine load_thres
#
# Purpose: load threshold data from file of devices that were previously down
########################################################################
sub load_thres {

   # Declare Local Variables
   my $oTime;           # Original timestamp when device went down
   my $lTime;           # Last timestamp when device was checked
   my $rTime;           # Recovery timestap when device recovered
   my $dTime;           # Amount of time the device has been down
   my $name;            # FQDN of the device being checked
   my $alert;           # Number of Pager alerts sent
   my $index;           # Index variable for hash array
   my $tmwindow = $currentTime - $thresholdTime;

   #
   # Load threshold data from \$thresholdfile for threshold checking
   #
   # We can use the pingsweeprecord.dat file to check for thresholds
   # The format of that file appears below. We can load all the data and then
   # count the number of events within the threshold window, perhaps 60 minutes.
   #
   # Device Hostname              Original   Recover   Total    Down       Date   Time
   #                                Time      Time      Time    Time 
   #---------------------------------------------------------------------------------------
   #
   # switchhostname.domain        1107277623 1107277801   238  0:00:03:58  2-1-2005 12:11:01
   #

   # Open data file
   open THRESDATA, "$thresholdfile" or die "Can't open $thresholdfile: $!\n";

   logit("DEBUG: (load_thres) starting to load hash \%thres",1) if (DEBUG);
   logit("DEBUG: (load_thres) threshold events = $thresholdEvents and time window = $tmwindow",1) if (DEBUG);

   # Walk through data file
   while (<THRESDATA>) {

      # Skip blank lines
      next if (/^\n$/);
      # Skip comments
      next if (/^#/);

      ($name, $oTime, $rTime, $dTime) = split(' ');

      # If the recovery time was before the \$thresholdTime lets count this event
      if ($rTime > ($currentTime - $thresholdTime) ) {
	 logit("DEBUG: (load_thres) threshold event for $name detected",1) if (DEBUG);
         $thres{$name}++;
      }

   } #end while

   close THRESDATA;

   # Lets dump all the devices that have reached the threshold 
   if (DEBUG) {
      logit("DEBUG: (load_thres) checking threshold events for each device", 1);
      for my $idx (sort keys %thres) {
         if ($thres{$idx} > $thresholdEvents) {
            logit("DEBUG: (load_thres) threshold event limit reached for $idx events = $thres{$idx}",1);
         }
      }
   }

return 1;

} #end sub load_thres

########################################################################
# Subroutine write_data
#
# Purpose: write list of devices not responding to file for future
########################################################################
sub write_data {
   # Declare Local variables
   my $name;
   my $oTime;
   my $key;
   my $now;
   my $lTime;
   my $dTime;
   my $alert;

   # Find time
   $now = time;

   # Open data file
   open DATA, ">$datafile";

   print DATA <<EOF;
#
# Device Hostname                   Original Time    Last Time   Down Time  Alert
#-----------------------------------------------------------------------------------
EOF

   #  for my $triple (sort byKeys (keys %data)) {
   for my $triple (sort keys %data) {
      ($name, $oTime, $lTime, $dTime, $alert) = split(' ', $data{$triple});

      # Calculate down time
      $dTime = &calc_down_time($oTime, $now);

      # Print formatting
      printf DATA "%-35s  %11s %11s %11s %4s\n",
                 $name, $oTime, $lTime, $dTime, $alert;

      logit("DEBUG: (write_data) writing to disk - $name $oTime $lTime $dTime $alert",1) if (DEBUG);

   } #end for my $tiple

   close DATA;

   return;

} #end sub write_data

##########################################################################
# Subroutine calc_down_time
#
# Purpose: calculate downtime given to time references
##########################################################################
sub calc_down_time {
   # Declare Local Variables
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

   } else {
      $iSecs  = 99;
      $iMins  = 99;
      $iHours = 99;
      $iDays  = 999;
   } #end if $cTime

   # Put it all together
   $iTime = "$iDays:$iHours:$iMins:$iSecs";

   return $iTime;
} #end sub calc_down_time

##########################################################################
# Subroutine comp_data
#
# Purpose: 
# Compare the current counts to those from the data file & update %disk
# After this routine runs, the information we've gathered and stored in
# %live has been merged into %disk
##########################################################################
sub comp_data {
   # Declare Local Variables
   my $oTime;          # original Time of event
   my $lTime;          # laset Time the status was reported
   my $dTime;		# amount of acruded downtime
   my $alert;
   my $diskoTime;      # Last time ifInOctets changed
   my $disklTime;      # Last time we asked device if ifInOctets
   my $diskalert;

   my $flag;
   my $name;               # Host from data file
   my $livename;
   my $downtime;

   $alert = 1;

   logit("DEBUG: (comp_data) checking values",1) if (DEBUG);

   # Walk through %live comparing the octet counter we just acquired with
   # the counter stored on disk
   for my $triple (keys %live) {

      # Get the current data
      $oTime = $live{$triple};

      # Grab an entry from the disk file
      if (exists $disk{$triple}) {
         ($name, $diskoTime, $disklTime, $dTime, $diskalert) = split (' ', $disk{$triple});
      } #end if

      # Compare
      if (! exists $disk{$triple} ) {             # New Device Down
         $flag = "new";
         $dTime = &calc_down_time($oTime, $currentTime);
         $data{$triple} = "$triple $oTime $currentTime $dTime $alert";
      } else  {                                   # Previously Down
         $flag = "activity";
         # Calculate idle time
         $dTime = &calc_down_time($oTime, $currentTime);
         $data{$triple} = "$triple $diskoTime $currentTime $dTime $diskalert";
      } #end if

   } #end for my $triple

   # This code will determine if previously down hosts are now recovered
   # and will set flags for the later subroutinues to alert on the reovery

   for my $test (sort keys %disk) {

      # Check to see if host has recovered (if still down then skip)
      next if (exists ($data{$test}));

      # Load data set for host from array
      ($name, $oTime, $lTime, $dTime) = split(' ', $disk{$test});

      # Threshold checking if device flapping do not set RECOVER flag

      # Not sure if this is exactly correct, how can we tell if we alerted
      # on this specific device before the threshold limit was reached or
      # not? If we alerted on this device should we not notify on recovery?
      # We would need someway to track every specific device and whether there
      # are any outstanding notifications (down report). Let's think about this...

      if ( exists( $thres{$name} ) && ($thres{$name} > $thresholdEvents)) {
         logit("DEBUG: (comp_data) threshold reached $name skipping recovery notification.",1) if (DEBUG);
         next;
      }

      # $dTime = &calc_down_time($oTime, $currentTime);
      #if ($currentTime - $oTime >= 60*2*1) {	# SECS*MINUTES*HOURS
      #CHANGED TIMING BECAUSE RECOVERY REPORTS WERE MISSING FOR DOWN REPORTS
      if ($currentTime - $oTime >= 30*1*1) {	# SECS*MINUTES*HOURS
         $RECOVER = 1;

         ($name, $oTime, $lTime, $dTime) = split(' ', $disk{$test});
         logit("DEBUG: (comp_data) the following host has recovered $name",1) if (DEBUG);
      } #end if $currentTime

   } #end for my $test
  

return 1;
} #end sub comp_data

############################################################################
# Subroutine alert_pager
#
# Purpose: send email alert to pager smtp address if applicable
############################################################################
sub alert_pager {
   # Declare Local Variables
   my $name;
   my $oTime;
   my $lTime;
   my $dTime;
   my $alert;

   # if number of devices alerting for more than 5 minutes is greater
   # than 10 then please page someone to alert them

   if ($PAGERNOTIFY >= 10) {

      logit("DEBUG: (alert_pager) issuing pager alert to $PAGERTO ($PAGERNOTIFY devices down)",1) if (DEBUG);

      open(SENDMAIL, "| /usr/lib/sendmail $PAGERTO") || die;

      print(SENDMAIL "From: $MAILFROM\nTo: $PAGERTO\nSubject: \n\n");

      print SENDMAIL "Date: $sdate";
      print SENDMAIL "There is a farily large problem going on at work.";
      print SENDMAIL "Approximately $PAGERNOTIFY devices have been down for more than 5 minutes.";

      close(SENDMAIL) || die;

   } else {

      logit("DEBUG: (alert_pager) not enough devices down - no pager alert ($PAGERNOTIFY devices down)",1) if (DEBUG); 

   } #endif ($NOTIFY)

   return;

} #end sub alert_pager

#############################################################################
# Subroutine check_alert
#
# Purpose: check to see what alerts are necessary and set flags
#############################################################################
sub check_alert {

   # Declare Local Variables
   my $index;
   my $name;
   my $oTime;
   my $lTime;
   my $dTime;
   my $alert;

   my $DownTimeToPage = 5*60;

   logit("DEBUG: (check_alert) checking for email notifications",1) if (DEBUG);

   $NOTIFY = 0;

   # Setting the ALERT value to 0 designates that this device should not be notified
   # if timedown > 5 minutes and number of devices > 10 alert by pager
   # if downtime < 5 minutes send email alert during every cycle
   # if timedown = 15 minutes send email alert
   # if timedown = 60 minutes send email alert 
   # if timedown > 60 minutes ignore

   for my $index (sort keys %data) {
      ($name, $oTime, $lTime, $dTime, $alert) = split(' ', $data{$index});

      # Threshold checking
      if ( exists( $thres{$name} ) && ($thres{$name} > $thresholdEvents)) {
         logit("DEBUG: (check_alert) threshold reached on device $name skipping alert notification.",1) if (DEBUG);
         next;
      }
   #
   # THIS SECTION WILL PERFORM THE LOGIC FOR DETERMINING THE NOTIFICATION TIMINGS
   #
   #      ALERT = 0	No futher notifications for this device
   #      ALERT = 1	first email notifycation (0 minute)
   #      ALERT = 2     single pager notification (5 minute)
   #      ALERT = 3	second email notifcation (15 minute)
   #      ALERT = 4	third email notification (1 hour)
   #
   # In the original script we notified immediately upon a failure, but that left
   # us with a lot of false positives, so then we notified only after the device
   # had been down for more than 30 seconds, but then that would lead to RECOVERY
   # notifications for devices that were never notified as being down.
   #
   # I've added some logic to retry the PING if the number of down devices is less
   # than 10. This will hopefully prevent the false positives for the one or two 
   # errand lost PINGs yet it won't create any performance issues or delay any
   # notifications if there is a larger problem that would go beyond a few lost or
   # errand PINGs.
   #
   ## I REMOVED THE LINE BELOW ON FEBRUARY 18, 2005 TO IMPROVE NOTIFICATIONS AND
   ## CONTROL THE NUMBER OF FALSE POSITIVE NOTIFICATIONS AND ALERTS 
   ##if (( $alert == 1 ) and ($currentTime - $oTime < 5*60) and ($currentTime - $oTime > 30)) {

   #REMOVED REQUIREMENT THAT DEVICE BE DOWN FOR > 60 SECONDS BEFORE ALERTING
   #if (( $alert == 1 ) and ($currentTime - $oTime < 5*60) ) {

      #if ( ( $alert == 1 ) and ($currentTime - $oTime < 5*60) ) {
      if ( ( $alert == 1 ) ) {
         logit("DEBUG: (check_alert) alerting on $name for 5 minutes",1) if (DEBUG);
         $data{$index} = "$name $oTime $lTime $dTime 2";
         $notify{$index} = "$name $oTime $lTime $dTime $alert";
         $NOTIFY = 1;
      ###} elsif (( $alert == 1) and ($currentTime - $oTime >= 2*60 )) {
      } elsif (( $alert == 2) and ($currentTime - $oTime >= $DownTimeToPage )) {
         logit("DEBUG: (check_alert) resetting alert for $name to 3",1) if (DEBUG);
         $data{$index} = "$name $oTime $lTime $dTime 3";
         $PAGERNOTIFY++;
         logit("DEBUG: (check_alert) pager alert now at $PAGERNOTIFY",1) if (DEBUG);
      } elsif (( $alert == 3) and ($currentTime - $oTime >= 60*15*1)) {	# SECS*MINUTES*HOURS
         logit("DEBUG: (check_alert) 15 minute alert for $name and clearing marker",1) if (DEBUG);
         $data{$index} = "$name $oTime $lTime $dTime 4";
         $notify{$index} = "$name $oTime $lTime $dTime $alert";
         $NOTIFY = 1;
      } elsif (( $alert == 4) and ($currentTime - $oTime >= 60*60*1)) {	# SECS*MINUTES*HOURS
         logit("DEBUG: (check_alert) 60 minute alert for $name and clearing marker",1) if (DEBUG);
         $data{$index} = "$name $oTime $lTime $dTime 0";
         $notify{$index} = "$name $oTime $lTime $dTime $alert";
         $NOTIFY = 1;
      }

   } #endfor $index

   return;

} #end sub check_alert


#############################################################################
# Subroutine record_downtime
#
# Purpose: check to see what alerts are necessary and set flags
#############################################################################
sub record_downtime {
   # Declare Local variables
   my $name;
   my $oTime;
   my $key;
   my $now;
   my $lTime;
   my $dTime;
   my $alert;
   my $tdtime;
   

   # Find time
   $now = time;

   if ($RECOVER) {
      
      logit("DEBUG: (record_downtime) there is something todo",1) if (DEBUG);

      # Open data file
      open DATA, ">>$recordfile";

#      print DATA <<EOF;
#
# Device Hostname              Original Time    Last Time   Total Time  Down Time Date Time
#------------------------------------------------------------------------------------------------
#EOF
#

      # processing in here for devices that recovered
      for my $triple (sort keys %disk) {

         next if ( exists ($data{$triple}) );

         ($name, $oTime, $lTime, $dTime) = split(' ', $disk{$triple});

         # Calculate down time
         $dTime = &calc_down_time($oTime, $currentTime);
         $tdtime = $currentTime - $oTime;

         printf DATA "%-30s  %10s %10s %10s %11s %9s %9s\n",
                 $name, $oTime, $lTime, $tdtime, $dTime, $date, $time;

         logit("DEBUG: (record_downtime) recording downtime to disk - $name $oTime $lTime $dTime",1) if (DEBUG);

      } #end for my $triple

      close(DATA);

   } #end if $RECOVER

   return;

} #end sub record_downtime

############################################################################
# Subroutine logit
#
# Purpose: log DEBUG and ERROR messages to console and log file
# 
#   Notes: there are two parameters, logit(error message, severity)
############################################################################
sub logit {

   # Declare Local Variables
   my $message = shift;			# the text of the actual message
   my $severity = shift;		# the severity of the message
   my $timedate = localtime;

   if (CONSOLE) {
      print "$timedate :: $message\n";
   }

   if (DEBUG) {
      print LOGFILE "$timedate :: $message\n";
   }

   return;

} #end sub logit

############################################################################
# Subroutine lockit
#
# Purpose: procedure to create lockfile or resolve abandon lockfile
#
# Notes: this subroutine was borrowed from Tobi Oetiker (MRTG)
############################################################################
sub lockit {
    my ($lockfile,$templock) = @_;

        open(LOCK,">$templock") or die "$NOW: ERROR: Creating templock $templock: $!";
        $main::Cleanfile = $templock;
        if (!link($templock,$lockfile)) { # Lock file exists - deal with it.
            my($nlink,$lockage) = (stat($lockfile))[3,9];
            $lockage = time() - $lockage;
            if ($nlink < 2 or $lockage > 30*60) { #lockfile is alone and old
                unlink($lockfile)
                  || do{ unlink $templock;
                         die "$NOW: ERROR: Can't unlink stale lockfile ($lockfile). Permissions?\n"};
                link($templock,$lockfile)
                  || do{ unlink $templock;
                         die "$NOW: ERROR: Can't create lockfile ($lockfile).\n".
                           "Permission problem or another process locking succesfully?\n"};
            } else {
                unlink $templock;
                die "$NOW: ERROR: It looks as if you are running two copies of process in parallel on\n".
                    "       the same config file. There is a lockfile ($lockfile) and it is\n".
                    "       is only $lockage seconds old ... Check your crontab.\n".
                    "       (/etc/crontab and /var/spool/cron/root) \n"
                        if $lockage < 4;

                die  "$NOW: ERROR: I guess another process is running. A lockfile ($lockfile) aged\n".
                     "$lockage seconds is hanging around. If you are sure that no other process\n".
                     "is running you can remove the lockfile\n";

        }
    }
    return;

} #end sub lockit