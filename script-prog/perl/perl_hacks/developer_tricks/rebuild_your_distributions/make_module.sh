#!/bin/bash

if [ -f Build.PL ]; then
    makeprog=Build
    makecommand="perl ./Build"
elif [ -f Makefile.PL ]; then
    makeprog=Makefile
    makecommand=make
else
    echo Nothing to reload!
    exit 1
fi
 
if [ -f $makeprog ]; then
    $makecommand realclean
fi
perl $makeprog.PL && $makecommand && $makecommand test
