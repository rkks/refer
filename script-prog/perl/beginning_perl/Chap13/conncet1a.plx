#!\usr\bin\perl
#connect1.plx

use warnings;
use strict;
use DBI;

{

#local $ENV{"DBI_DRIVER"} = "mysql";
local $ENV{"DBI_DSN"} = "test";
local $ENV{"DBI_USER"} = "root";
local $ENV{"DBI_PASS"} = "";



my $dbh=DBI->connect('dbi::') || 
   die "Error opening database: $DBI::errstr\n";
print "Hello";
$dbh->disconnect;
print "Goodbye";
}