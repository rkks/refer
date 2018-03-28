#!/bin/bash
#########################################################################
# ciscoRange.sh
# 
#    This is a shell that loops inside a file with IPs, executing
#    the list and using the variables in the expect command vty_runcmd.exp
#
# Adapted by Sergio Zavala (sergio.zavala at sidetec.com.mx),  Feb/2011
#
# Syntax:
# [network@network bin]$ ./ciscoFile.sh MY_IPLIST_FILE.TXT
#
# HISTORY:
#   Sergio Zavala	02/04/11 First Version 
##########################################################################

ruser="admin"
rpass="nbv_12345"
renable="enable"
cmdList="commandlist.txt"

./vty_runcmd.exp -h "10.197.154.43" -u $ruser -p $rpass -e $renable -f ./$cmdList > $line.cfg

#while read line
#do
#if [ $line != "" ]; then
#  echo ""
#  echo "::::::::::::::::::::::::::::::::"
#  echo $line
#  echo "::::::::::::::::::::::::::::::::"
#  
# Calling the expect script
#  ./vty_runcmd.exp -h $line -u $ruser -p $rpass -e $renable -f ./$cmdList > $line.cfg
#fi
#done < $1

exit 0

