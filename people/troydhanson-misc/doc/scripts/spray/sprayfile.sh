#!/bin/bash
FILE=$1
DST=$2
if [ ! -f "$FILE" ]
then
  echo "usage: $0 <file> <dstpath>"
  exit -1
fi
if [ -z "$DST" ]
then
  echo "usage: $0 <file> <dstpath>"
  exit -1
fi

while read ip
do
  echo $ip
  scp ${FILE} $ip:${DST}
done<ip.txt
