#!\usr\bin\perl
#connect1.plx

use warnings;
use strict;
use DBI;

my $host='localhost';
my $port='3306';
my $dsn='dbi:mysql:test';
my $user='root';
my $pass='';


$ENV{DBI_AUTOPROXY}="host=$host;port=$port";

my $dbh=DBI->connect($dsn,$user,$pass) ||
   die "Error opening database: $DBI::errstr\n";
print "Hello";
$dbh->disconnect;
print "Goodbye";