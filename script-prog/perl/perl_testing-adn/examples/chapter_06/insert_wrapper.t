#!perl
  
use strict;
use warnings;
  
use IO::Scalar;
  
use Test::More tests => 15;
use DBD::Mock;
use Test::Exception;
  
my $module      = 'InsertWrapper';
use_ok( $module ) or exit;
  
my $log_message = '';
my $fh          = IO::Scalar->new( \$log_message );
my $drh         = DBI->install_driver( 'Mock' );
  
can_ok( $module, 'new' );
  
$drh->{mock_connect_fail} = 1;
  
my %args = ( dsn => 'dbi:Mock:', logfh => $fh, user => '', password => '' );
throws_ok { $module->new( %args ) } qr/Could not connect/,
    'new(  ) should fail if DB connection fails';
  
$drh->{mock_connect_fail} = 0;
my $wrap;
lives_ok { $wrap = $module->new( %args ) }
    '... or should succeed if connection works';
isa_ok( $wrap, $module );
  
like( $log_message, qr/CONNECT/,            '... logging connect message' );
like( $log_message, qr/\tdsn => $args{dsn}/, '... with dsn'               );
$log_message = '';
  
can_ok( $module, 'dbh' );
isa_ok( $wrap->dbh(  ), 'DBI::db' );
  
can_ok( $module, 'insert' );
$wrap->dbh(  )->{mock_can_connect} = 0;
  
throws_ok { $wrap->insert( 'users', name => 'Jerry', age => 44 ) }
    qr/prepare failed/,
    'insert(  ) should throw exception if prepare fails';
  
$wrap->dbh(  )->{mock_can_connect} = 1;
lives_ok { $wrap->insert( 'users', name => 'Jerry', age => 44 ) }
    '... but should continue if it succeeds';
  
like( $log_message, qr/INSERT/,          '... logging insert message' );
like( $log_message, qr/\tname => Jerry/, '... with inserted data'     );
like( $log_message, qr/\tage => 44/,     '... for each column'        );
