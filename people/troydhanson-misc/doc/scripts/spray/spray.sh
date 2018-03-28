#!/bin/bash
CMD=$1
while read ip
do
  echo $ip
  ssh -n $ip "$CMD"
done<ip.txt
