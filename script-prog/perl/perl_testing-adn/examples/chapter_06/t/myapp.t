#!perl
  
BEGIN
{
    chdir 't' if -d 't';
}
  
use strict;
use warnings;
  
use Test::More 'no_plan'; # tests => 1;
  
use DBI;
use My::App::Config;
  
my $user    = My::App::Config->config( 'user'     );
my $pass    = My::App::Config->config( 'password' );
my $db_name = My::App::Config->config( 'test_db'  );
my $db_type = My::App::Config->config( 'db_type'  );
  
my $dbh     = DBI->connect( "DBI:$db_type:dbname=$db_name", $user, $pass );
  
my $module  = 'My::App';
use_ok( $module ) or exit;
