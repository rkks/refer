#!/bin/bash
#########################################################################
# ciscoRange.sh
# 
#    This is a shell that loops in a range of consecutive IPs with 
#    the expect command vty_runcmd.exp
#
# Adapted by Sergio Zavala (sergio.zavala at sidetec.com.mx),  Feb/2011
#
# First arg is the begin ip, the second one is the last octet of the end ip 
# Syntax:
# [network@network bin]$ ./ciscoRange.sh 192.168.2.1 10
#
# HISTORY:
#   Sergio Zavala	02/04/11 First Version 
##########################################################################

# highest valid IP octet value
MaxValue=255

ip="10.197.154.43"      #$1
endip="10.197.154.43"   #$2
ruser="admin"
rpass="nbv_12345"
renable="enable"
cmdList="commandlist.txt"

baseaddr="$(echo $ip | cut -d. -f1-3)"
lsv="$(echo $ip | cut -d. -f4)"

while [ $endip -ge $lsv ] 
do
   if [ $lsv -eq $MaxValue ] ; then
    echo "edge case not implemented"
	exit 1
  fi
  echo ""
  echo "::::::::::::::::::::::::::::::::"
  echo $baseaddr.$lsv
  echo "::::::::::::::::::::::::::::::::"
  
# Calling the expect script
  ./vty_runcmd.exp -h $baseaddr.$lsv -u $ruser -p $rpass -e $renable -f ./$cmdList > $baseaddr.$lsv.cfg

  lsv=$(( $lsv + 1 ))
done

exit 0
