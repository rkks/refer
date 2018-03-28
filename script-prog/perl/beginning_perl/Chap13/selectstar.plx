#!\usr\bin\perl
#selectstar.plx

use warnings;
use strict;
use DBI;

my ($dbh, $sth);

$dbh=DBI->connect('dbi:mysql:test','root','elephant') || 
   die "Error opening database: $DBI::errstr\n";

$sth=$dbh->prepare("SELECT * from checkin WHERE checkedin IS NOT NULL;") ||
   die "Prepare failed: $DBI::errstr\n";

$sth->execute() ||
   die "Couldn't execute query: $DBI::errstr\n";

my $matches=$sth->rows();          
unless ($matches) {
   print "Sorry, there are no matches\n";
} else {
   print "$matches matches found:\n";
        while (my @row = $sth ->fetchrow_array) {
            print "@row\n";
        }
     }
}

$sth->finish();

$dbh->disconnect || die "Failed to disconnect\n";
