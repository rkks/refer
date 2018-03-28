#!/bin/bash

if [ ! -x ./sines ]
then
  echo 'run "make" before this script'
  exit -1
fi

echo Dial tone;   ./sines -s 1 350 440
echo Busy signal; ./sines -s 1 620 480


#  DTMF (phone buttons - each comprised of two frequencies)
#          1209 Hz 1336 Hz 1477 Hz 1633 Hz
#  697 Hz  1       2       3       A
#  770 Hz  4       5       6       B
#  852 Hz  7       8       9       C
#  941 Hz  *       0       #       D

# play each of the telephone button sounds

echo button 1; ./sines -s 1 1209 697 
echo button 2; ./sines -s 1 1336 697 
echo button 3; ./sines -s 1 1477 697 
echo button 4; ./sines -s 1 1209 770 
echo button 5; ./sines -s 1 1336 770 
echo button 6; ./sines -s 1 1477 770 
echo button 7; ./sines -s 1 1209 852 
echo button 8; ./sines -s 1 1336 852 
echo button 9; ./sines -s 1 1477 852 
echo button \*; ./sines -s 1 1209 941 
echo button 0; ./sines -s 1 1336 941 
echo button \#;./sines -s 1 1477 941 
