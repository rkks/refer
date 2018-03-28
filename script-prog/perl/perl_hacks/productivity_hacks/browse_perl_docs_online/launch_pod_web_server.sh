#!/bin/sh

podwebserver &
sleep 2
firefox -remote 'openurl( http://localhost:8020, new-tab)'
