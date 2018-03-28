#!\usr\bin\perl
#connect1.plx

use warnings;
use strict;
use DBI;

my $dbh=DBI->connect('dbi:mysql:test','root','') || 
   die "Error opening database: $DBI::errstr\n";
print "Hello\n";
$dbh->disconnect || die "Failed to disconnect\n";
print "Goodbye\n";