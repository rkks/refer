#!perl
  
use lib 'lib';
  
use strict;
use warnings;
  
use DBI;
  
my $dbh = DBI->connect( 'dbi:SQLite:dbname=test_data' );
{
    local $/ = ";\n";
    $dbh->do( $_ ) while <DATA>;
}
  
use Test::More tests => 4;
use Test::DatabaseRow;
  
my $module = 'Users';
use_ok( $module ) or exit;
$module->set_db( $dbh );
$module->create( name => 'Emily', age => 23 );
  
local $Test::DatabaseRow::dbh = $dbh;
  
row_ok(
    sql   => 'SELECT count(*) AS count FROM users',
    tests => [ count => 3 ],
    label => 'create(  ) should insert a row',
);
  
row_ok(
    table   => 'users',
    where   => [ name => 'Emily', age => 23 ],
    results => 1,
    label   => '... with the appropriate data',
);
  
row_ok(
    table => 'users',
    where => [ id => 3 ],
    tests => [ name => 'Emily', age => 23 ],
    label => '... and a new id',
);
  
__END__
BEGIN TRANSACTION;
DROP TABLE users;
CREATE TABLE users (
    id   int,
    name varchar(25),
    age  int
);
INSERT INTO "users" VALUES(1, 'Randy', 27);
INSERT INTO "users" VALUES(2, 'Ben', 29);
COMMIT;
