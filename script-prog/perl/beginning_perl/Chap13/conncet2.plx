#!\usr\bin\perl
#connect1.plx

use warnings;
use strict;
use DBI;

my $dbh=DBI->connect('dbi:mysql:test:localhost','root','') ||
   die "Error opening database: $DBI::errstr\n";
print "Hello";
$dbh->disconnect;
print "Goodbye";