#!/usr/bin/perl
#simpledb.plx
use warnings;
use strict;
use POSIX;
use SDBM_File;           # or GDBM_File / NDBM_File / AnyDBM_File...

my %dbm;
my $db_file = "simpledb.dbm";

tie %dbm, 'SDBM_File', $db_file, O_CREAT|O_RDWR, 0644;

if (tied %dbm) {
	print "File $db_file now open.\n";
} else {
	die "Sorry - unable to open $db_file\n";
}

$_ = "";

until (/^q/i) {

print "What would you like to do? ('o' for options): ";
chomp($_ = <STDIN>);

if ($_ eq "o") { dboptions() }
elsif ($_ eq "r") { readdb() }
elsif ($_ eq "l") { listdb() }
elsif ($_ eq "w") { writedb() }
elsif ($_ eq "d") { deletedb() }
elsif ($_ eq "x") { cleardb() }
else { print "Sorry, not a recognized option.\n"; }
}

untie %dbm;

#*** Option Subs ***#

sub dboptions {
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

sub readdb {
	my $keyname = getkey();

	if (exists $dbm{"$keyname"}) {
		print "Element '$keyname' has value $dbm{$keyname}";
	} else {
		print "Sorry, this element does not exist.\n"
	}
}

sub listdb {
	foreach (sort keys(%dbm)) {
		print "$_ => $dbm{$_}\n";
	}
}

sub writedb {
	my $keyname = getkey();
	my $keyval = getval();

	if (exists $dbm{$keyname}) {
		print "Sorry, this element already exists.\n"
	} else {
		$dbm{$keyname}=$keyval;
	}
}

sub deletedb {
	my $keyname = getkey();

	if (exists $dbm{$keyname}) {
		print "This will delete the entry $keyname.\n";
		delete $dbm{$keyname} if besure();
	}
}

sub cleardb {
	print "This will delete the entire contents of the current database.\n";
	undef %dbm if besure();
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

sub besure {
	print "Are you sure you want to do this?";
	$_ = <STDIN>;
	/^y/i;
}