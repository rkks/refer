#!\usr\bin\perl
#query1.plx

use warnings;
use strict;
use DBI;

my ($dbh, $sth, $name, $id);

$dbh=DBI->connect('dbi:mysql:test','root','') || 
   die "Error opening database: $DBI::errstr\n";

$sth=$dbh->prepare("SELECT * from testac;");

$sth->execute();			# execute the statement

my $matches=$sth->rows(); 			
unless ($matches) {
	print "Sorry, there are no matches\n";
} else {
	print "$matches matches found:\n";
        while (( $id, $name) = $sth ->fetchrow_array) {
            print "$name has ID $id\n";
        }
}

$sth->finish();				# finish the execution


$dbh->disconnect || die "Failed to disconnect\n";
