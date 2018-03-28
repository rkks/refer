#!\usr\bin\perl
#insert1.plx

use warnings;
use strict;
use DBI;

my ($dbh, $rows);

$dbh=DBI->connect('dbi:mysql:test','root','') || 
   die "Error opening database: $DBI::errstr\n";

$rows=$dbh->do("INSERT INTO checkin (firstname, lastname, destination)
               VALUES              ('John',    'Smith',  'Glasgow')") ||
           die "Couldn't insert record : $DBI::errstr";

print "$rows added to checkin";

$dbh->disconnect || die "Failed to disconnect\n";
