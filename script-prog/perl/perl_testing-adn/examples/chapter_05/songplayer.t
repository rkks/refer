#!perl
  
use strict;
use warnings;
  
use lib 'lib';
  
use Test::More tests => 11;
use Test::Exception;
  
my $module = 'SongPlayer';
use_ok( $module ) or exit;
  
can_ok( $module, 'new' );
my $song = $module->new( song => 'RomanceMe.mp3', player => 'xmms' );
isa_ok( $song, $module );
  
can_ok( $song, 'song' );
is( $song->song(  ), 'RomanceMe.mp3',
    'song(  ) should return song set in constructor' );
  
can_ok( $song, 'player' );
is( $song->player(  ), 'xmms',
    'player(  ) should return player set in constructor' );
  
can_ok( $song, 'play' );
  
{
    package SongPlayer;
  
    use subs 'system';
  
    package main;
  
    my $fail = 0;
    my @args;
  
    *SongPlayer::system = sub
    {
        @args = @_;
        return $fail;
    };
  
    lives_ok { $song->play(  ) } 'play(  ) should live if launching succeeds';
  
    is_deeply( \@args, [qw( xmms RomanceMe.mp3 )],
        'play(  ) should launch player for song' );
  
    $fail = 1;
    throws_ok { $song->play(  ) } qr/Couldn't launch xmms for RomanceMe.mp3/,
        '... throwing exception if launching fails';
}
