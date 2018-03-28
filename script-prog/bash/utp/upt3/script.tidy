#!/bin/sh
# Public domain.

# Put CTRL-M in $m and CTRL-H in $b.
# Change \010 to \177 if you use DEL for erasing.
eval `echo m=M b=H | tr 'MH' '\015\010'`

exec sed "s/$m\$//
:x
s/[^$b]$b//
t x" $*
