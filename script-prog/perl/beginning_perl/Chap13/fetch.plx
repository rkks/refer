#!\usr\bin\perl


use warnings;
use strict;
use DBI;

my ($dbh, $sth);



sub count_table {
  my ($dbh,$table,$sql,@values)=@_;

  $sql="" unless defined $sql; 

  my $sth=$dbh->prepare("SELECT COUNT(*) FROM $table $sql") ||
          die "Prepare failed: $DBI::errstr\n";

  $sth->execute(@values) || die "Execute failed\n";	      # execute the statement

  # return the result of the count
  return ($sth->fetchrow_array())[0];
}



$dbh=DBI->connect('dbi:mysql:test','root','') || 
   die "Error opening database: $DBI::errstr\n";

print count_table($dbh,"checkin");
print "\n";
print count_table($dbh,"checkin","WHERE destination='San Diego'");
print "\n";
print count_table($dbh,"checkin","WHERE destination=?","Japan");


$sth->finish();				# finish the execution


$dbh->disconnect || die "Failed to disconnect\n";



