#!/usr/bin/perl
# backupkill.plx
use warnings;
use strict;
use File::Find;
find ( \&callback, "/") ;

sub callback {
   unlink $_ if /\.bak$/;
}
