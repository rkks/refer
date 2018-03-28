#!/usr/bin/perl
#Ex9_1.plx

my %hash;

$_ = "";

until (/^q/i) {

print "What would you like to do? ('o' for options): ";
chomp($_ = <STDIN>);

if ($_ eq "o") { options() }
elsif ($_ eq "r") { readit() }
elsif ($_ eq "l") { listit() }
elsif ($_ eq "w") { writeit() }
elsif ($_ eq "d") { deleteit() }
elsif ($_ eq "x") { clearit() }
elsif ($_ eq "q") { print "Bye!\n"; }
else { print "Sorry, not a recognized option.\n"; }
}

sub options {
   print<<EOF;
      Options available:
      o - view options
      r - read entry
      l - list all entries
      w - write entry
      d - delete entry
      x - delete all entries
EOF
}

sub readit {
   my $keyname = getkey();

   if (exists $hash{"$keyname"}) {
      print "Element '$keyname' has value $hash{$keyname}";
   } else {
      print "Sorry, this element does not exist.\n"
   }
}

sub listit {
   foreach (sort keys(%hash)) {
      print "$_ => $hash{$_}\n";
   }
}

sub writeit {
   my $keyname = getkey();
   my $keyval = getval();

   if (exists $hash{$keyname}) {
      print "Sorry, this element already exists.\n"
   } else {
      $hash{$keyname}=$keyval;
   }
}

sub deleteit {
   my $keyname = getkey();

   if (exists $hash{$keyname}) {
      print "This will delete the entry $keyname.\n";
      delete $hash{$keyname} if besure();
   }
}

sub clearit {
   print "This will delete the entire contents of the current database.\n";
   undef %hash;
}

#*** Input Subs ***#

sub getkey {
   print "Enter key name of element: ";
   chomp($_ = <STDIN>);
   $_;
}

sub getval {
   print "Enter value of element: ";
   chomp($_ = <STDIN>);
   $_;
}
