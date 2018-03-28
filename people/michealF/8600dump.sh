#!/bin/sh
#
# Filename: /usr/local/etc/8600dump.sh
#
# Purpose:  Dump technical information from Nortel Ethernet Routing Switch
#           via telneting to the device and issuing various "show" cmds.
#           The output will then be saving to the working directory using
#           a filename based on the switch name used to call the script.
#
# Language: Bash Script
#
# Author:   Michael McNamara
#
# Date:     May 6, 2003
#
# Changes:
#
#   Sept 29, 2006: cleaned up script/updated documenation
#    Dec 30, 2005: added command line arguements for portability
#    Mar 18, 2005: added file logging for troubleshooting and monitoring
#    May 20, 2003: fine tuned script removing a great many "expect" commands.
#    May  6, 2003: original Expect script generated from auto_expect
#
# Notes:
#        Command Line Reference;
#          ./8600dump.sh 
#
# There are system and network specific variables below. Obviously the "PATH"
# to the location of the Bash script and supporting Expect script. The location
# of MUTT if email is used and most importantly the username and password to the
# Nortel Ethernet Routing Switch 8600. I would highly suggest using the ro (ReadOnly)
# account for all scripting purposes that are "read-only" in nature. The last most 
# obvious piece is the list of switches that you'd like the script run against.
#

# Global Variables
PATH_TO=/usr/local/etc/mlh
DUMP=8600dump.exp
MAIL_LIST=''
PAGER_LIST=''
ERROR_FLAG=0
MAILEXE='/usr/bin/mutt'
MAILTXT='/tmp/mutt.txt'
LOCKFILE=/tmp/trace.lck
USERNAME=ro
PASSWORD=

SWITCHES='switch1.domain switch2.domain'

#############################################################################
#  B E G I N   M A I N
#############################################################################

for SWITCH in $SWITCHES
do
	$PATH_TO/$DUMP $SWITCH $USERNAME $PASSWORD
	$MAILEXE -s "ALERT: Dump Report for $SWITCH" $MAIL_LIST -a $PATH_TO/$SWITCH.trace.log < $MAILTXT
done

exit 
#############################################################################
#  E N D   M A I N
#############################################################################

