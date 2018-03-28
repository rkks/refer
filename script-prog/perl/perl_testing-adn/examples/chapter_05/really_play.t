#!perl
  
use strict;
use warnings;
  
use lib 'lib';
  
use Test::More tests => 5;
use Test::Exception;
  
my $module = 'SongPlayer';
use_ok( $module ) or exit;
  
my $song = $module->new( song => '77s_OneMoreTime.ogg',
    player => 'mpg321' );
  
$song->song( 'pass.pl' );
is( $song->song(  ), 'pass.pl',
    'song(  ) should update song member, if set' );
  
$song->player( $^X );
is( $song->player(  ), $^X,
    'player(  ) should update player member, if set' );
  
lives_ok { $song->play(  ) } 'play(  ) should launch program and live';
  
$song->song( 'fail.pl' );
dies_ok { $song->play(  ) }
    'play(  ) should croak if program launch fails';
