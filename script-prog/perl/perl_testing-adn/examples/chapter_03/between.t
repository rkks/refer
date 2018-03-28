#!perl
  
use strict;
use warnings;
  
use Test::Between;
use Test::Builder::Tester tests => 3;
  
my $desc;
  
$desc = 'simple alphabetical comparison';
test_pass( $desc );
is_between( 'b', 'a', 'c',  $desc );
test_test( $desc );
  
$desc = 'simple numeric comparison';
test_pass( $desc );
is_between(  2,  1,  3, $desc );
test_test( $desc );
  
$desc = 'mixed comparison';
test_out( "not ok 1 - $desc" );
test_fail( +2 );
test_diag( '        two is not between 1 and 3' );
is_between(  'two',  1,  3, $desc               );
test_test( 'failed comparison with diagnostics' );
