#!/bin/sh
# pick:  select arguments

PATH=/bin:/usr/bin

for i
do
    echo -n "$i? " >/dev/tty
    read response
    case $response in
    y*)    echo $i ;;
    q*)    break
    esac
done </dev/tty
