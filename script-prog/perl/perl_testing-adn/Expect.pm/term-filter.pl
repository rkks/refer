#!/usr/local/bin/perl
while (<>) { s/\033\[.*?[a-zA-Z]//g; print; }
