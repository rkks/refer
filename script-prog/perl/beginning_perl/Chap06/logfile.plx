#!/usr/bin/perl
#logfile.plx
use warnings;
use strict;

my $logging = "screen";        # Change this to "file" to send the log to a file!

if ($logging eq "file") {
   open LOG, "> output.log" or die $!;
   select LOG;
}

print "Program started: ", scalar localtime, "\n";
sleep 30;
print "Program finished: ", scalar localtime, "\n";

select STDOUT;



