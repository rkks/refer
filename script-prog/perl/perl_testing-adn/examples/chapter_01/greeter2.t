#!perl
  
use strict;
use warnings;
  
use Test::More tests => 6;
  
use_ok( 'Greeter' ) or exit;
  
my $greeter = Greeter->new( name => 'Emily', age => 21 );
isa_ok( $greeter, 'Greeter' );
  
is(   $greeter->age(  ),   21,
    'age(  ) should return age for object' );
like( $greeter->greet(  ), qr/Hello, .+ is Emily!/,
    'greet(  ) should include object name' );
  
is(   $greeter->age(  ),   22,
    'Emily just had a birthday' );
like( $greeter->greet(  ), qr/Howdy, pardner!/,
    '... and she talks like a cowgirl' );
