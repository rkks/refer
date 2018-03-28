#!/bin/sh
#
# Filename: /usr/local/etc/backup-symbolswitches.sh
#
# Purpose:  Shell script that will launch the Expect script that will backup
#           the Symbol Wireless LAN Switch. This script will also notify
#           based on the results of each backup.
#
# Switches: Symbol 5000 Wireless LAN Switch (v1.4.3-x)
#           Symbol 5100 Wireless LAN Switch (v1.4.3-x)
#
# Author:   Michael McNamara 
#
# Date:     June 1, 2005
#
# Version:  0.5
#
# Changes:
#           June 8, 2005 (M.McNamara)
#           add documentation and ARGV command line checks
#
#
##############################################################################
#

LPATH=/usr/local/etc
EXE=backup-symbolswitches.exp
OUT=/tmp/symbolbackuplog.txt
DATE=`date`

MAIL_TO=''
MAIL_FROM=''
MAIL_SUBJECT='Symbol Wireless LAN Switch Backups'
SENDMAIL="/usr/sbin/sendmail"

cat /dev/null > $OUT

echo "To: $MAIL_TO" >> $OUT
echo "From: $MAIL_FROM" >> $OUT
echo "Subject: $MAIL_SUBJECT" >> $OUT
echo "MIME-Version: 1.0" >> $OUT
echo "Content-Type: text/html; charset=us-ascii" >> $OUT
echo >> $OUT
echo >> $OUT
echo "<P><FONT FACE=VERDANA SIZE=2>" >> $OUT
echo "<IMG HEIGHT=50 SRC=http://madmax.mdc.mlhs.org/image6.gif WIDTH=175 BORDER=0 ALIGN=right></P>" >> $OUT
echo "<B>Main Line Health $MAIL_SUBJECT</B><BR>" >> $OUT
echo >> $OUT
echo "Date: $DATE<BR>" >> $OUT
echo >> $OUT
echo "<P><FONT FACE=VERDANA SIZE=2>" >> $OUT
echo >> $OUT
echo "This is an automated message concerning the status of the automated Symbol Wireless LAN Switch" >> $OUT
echo "backups." >> $OUT
echo "<P>" >> $OUT

#
# You could use a loop here to iterate over the switches but you'll need
# a unique filename for each switch. We generally use the FQDN appended 
# with ".cfg" but it seems that the WS5000/WS5100s have issues with long
# filesname so we just took the lazy approach and hardcoded the filesnames
# into the shell script.
#

$LPATH/$EXE sw-wireless.domain sw-wireless-domain.cfg >> $OUT

echo "<P>" >> $OUT
echo "<FONT FACE=VERDANA SIZE=2>Notes: this message is now being sent in HTML format.<BR>" >> $OUT
echo "</font>" >> $OUT
echo "<P>" >> $OUT
echo "<IMG SRC=http://ziggy.mdc.mlhs.org/powered_by_perl.gif><BR>" >> $OUT
echo "<IMG SRC=http://ziggy.mdc.mlhs.org/powered_by_redhat.gif><BR>" >> $OUT

$SENDMAIL $MAIL_TO < $OUT

# End
