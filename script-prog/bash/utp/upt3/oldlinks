#!/bin/sh
# oldlinks - find unconnected symbolic links in the current directory and below
# Usage: oldlinks

find . -type l -print | perl -nle '-e || print'
