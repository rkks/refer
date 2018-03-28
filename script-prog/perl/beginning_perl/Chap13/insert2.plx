#!\usr\bin\perl
#insert1.plx

use warnings;
use strict;
use DBI;

my ($dbh, $sth, $firstname, $lastname, $destination, $rows);

$dbh=DBI->connect('dbi:mysql:test','root','') || 
   die "Error opening database: $DBI::errstr\n";

$sth=$dbh->prepare
             ("INSERT INTO checkin (firstname, lastname, destination)
               VALUES              (?        , ?       , ?          )");

$rows=0;

while (<>) {
  chomp;
  ($firstname, $lastname, $destination) = split(/:/);
  $sth->execute($firstname, $lastname, $destination) 
      || die "Couldn't insert record : $DBI::errstr";

  $rows+=$sth->rows();
}


print "$rows new rows have been added to checkin";

$dbh->disconnect || die "Failed to disconnect\n";
