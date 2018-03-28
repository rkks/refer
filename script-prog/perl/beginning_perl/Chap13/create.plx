#!\usr\bin\perl
#create.plx

use warnings;
use strict;
use DBI;

my ($dbh, $sth);

$dbh=DBI->connect('dbi:mysql:test','root','elephant') || 
   die "Error opening database: $DBI::errstr\n";

$sth=$dbh->prepare("CREATE TABLE checkin (
          id        INTEGER AUTO_INCREMENT PRIMARY KEY,
         firstname     VARCHAR(32) NOT NULL,
         lastname     VARCHAR(32) NOT NULL,
         checkedin     INTEGER,
         numberofbags   INTEGER,
         destination     VARCHAR(32) NOT NULL)");

$sth->execute();           # execute the statement

$sth->finish();            # finish the execution
print "All done\n";
$dbh->disconnect || die "Failed to disconnect\n";
