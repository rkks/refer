#!\usr\bin\perl


use warnings;

use DBI;

my ($dbh, $sth);

$dbh=DBI->connect('dbi:mysql:test','root','') || 
   die "Error opening database: $DBI::errstr\n";

sub get_first_row {
	my ($dbh,$table,$columns,$results,$sql,@values)=@_;
        my $col_list;

	$sql="" unless defined $sql; #suppress undef warnings

        $col_list = join(',',@{$columns});
	my $sth=$dbh->prepare("
		SELECT $col_list
		FROM $table 
		$sql 
		LIMIT 1
	") or return undef;
$sth->execute(@values) or return undef;

print "NF ", $sth->{'NUM_OF_FIELDS'}, "\n";
print "FCN ", $sth->{'NAME'}->[0], "\n";

my @upper_cased_names=$sth->{'NAME_uc'};
my @lower_cased_names=$sth->{'NAME_lc'};

$sth->bind_columns(@{$results}) if defined $results;

return $sth->fetchrow_array; #return array;
}

my ($first,$last);
my @columns=('firstname','lastname');
my @return_values=(\$first,\$last);
get_first_row($dbh,"checkin",\@columns,\@return_values);

print "Match: $last, $first\n";



$sth->finish();				# finish the execution


$dbh->disconnect || die "Failed to disconnect\n";



