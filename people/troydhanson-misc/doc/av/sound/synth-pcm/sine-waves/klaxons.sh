#!/bin/bash

if [ ! -x ./sines ]
then
  echo 'run "make" before this script'
  exit -1
fi

./sines -s 1 200/8000 2/32000
./sines -s 1 5/30000 500/10000
./sines -s 1 1/30000 200/10000 500/10000
