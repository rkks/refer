#!/usr/bin/perl
# Ex6_2.plx
use warnings;
use strict;

my $target;
while (1) {
   print "What file should I write on? ";
   $target = <STDIN>;
   chomp $target;
   if (-d $target) {
      print "No, $target is a directory.\n";
      next;
   }
   if (-e $target) {
      print "File already exists. What should I do?\n";
      print "(Enter 'r' to write to a different name, ";
      print "'o' to overwrite or\n";
      print "'b' to back up to $target.old)\n";
      my $choice = <STDIN>;
      chomp $choice;
      if ($choice eq "r") {
         next;
      } elsif ($choice eq "o") {
         unless (-o $target) {
            print "Can't overwrite $target, it's not yours.\n";
            next;
         }
         unless (-w $target) {
            print "Can't overwrite $target: $!\n";
            next;
         }
      } elsif ($choice eq "b") {
      if (-e $target.".old") {
              print "Backup already exists. Overwrite?\n";
         my $choice = <STDIN>;
         chomp $choice;
         next unless ($choice eq "y");
      } elsif ( rename($target,$target.".old") ) {
         print "OK, moved $target to $target.old\n";
      } else {
         print "Couldn't rename file: $!\n";
         next;
      }
      } else {
         print "I didn't understand that answer.\n";
         next;
      }
   }
   last if open OUTPUT, "> $target";
   print "I couldn't write on $target: $!\n";
   # and round we go again.
}
print OUTPUT "Congratulations.\n";
print "Wrote to file $target\n";
