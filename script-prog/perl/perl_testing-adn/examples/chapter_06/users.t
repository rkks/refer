#!perl
  
use strict;
use warnings;
  
use DBI;
  
my $dbh = DBI->connect( 'dbi:SQLite:dbname=test_data' );
{
    local $/ = ";\n";
    $dbh->do( $_ ) while <DATA>;
}
  
use Test::More tests => 10;
  
my $module = 'Users';
use_ok( $module ) or exit;
  
can_ok( $module, 'set_db' );
$module->set_db( $dbh );
  
can_ok( $module, 'fetch'  );
my $user = $module->fetch( id => 1 );
isa_ok( $user, 'User' );
is( $user->name(  ), 'Randy', 'fetch(  ) should fetch proper user by id' );
  
$user    = $module->fetch( name => 'Ben' );
is( $user->id(  ), 2, '... or by name' );
  
can_ok( $module, 'create' );
$user    = $module->create( name => 'Emily', age => 23 );
isa_ok( $user, 'User' );
is( $user->name(  ), 'Emily', 'create(  ) should create and return new User' );
is( $user->id(  ), 3, '... with the correct id' );
  
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
