#!perl
  
use strict;
use warnings;
  
use Library;
use Music;
  
use Test::More tests => 13;
use Test::Exception;
use Test::MockObject::Extends;
  
my $lib      = Library->new( 'my_files' );
my $mock_lib = Test::MockObject::Extends->new( $lib );
  
my $module   = 'Jukebox';
use_ok( $module ) or exit;
  
can_ok( $module, 'new' );
my $jb = $module->new( $mock_lib );
isa_ok( $jb, $module );
  
can_ok( $jb, 'library' );
is( $jb->library(  ), $mock_lib,
    'library(  ) should return library set in constructor' );
  
can_ok( $jb, 'play_music' );
  
$mock_lib->set_always( -path => 'my_path' );
throws_ok { $jb->play_music( 'mp3', 'Romance Me' ) } qr/Unknown directory/,
    'play_music(  ) should throw exception if it cannot find directory';
  
$mock_lib->set_always( -find_dir => 'my_directory' );
$mock_lib->set_always( read_file => { file => 'my_file' } );
$mock_lib->set_true( 'save_file' );
  
lives_ok { $jb->play_music( 'CD', 'Films For Radio' ) }
    '... but no exception if it can find it';
  
$mock_lib->called_ok( 'read_file' );
my ($method, $args) = $mock_lib->next_call( 2 );
is( $method,    'save_file',       'play_music(  ) should also save file' );
is( $args->[1], 'my_directory',    '... saving to the proper directory' );
is( $args->[2], 'Films For Radio', '... with the proper id'             );
is( $args->[3]{count}, 1,          '... and the proper count'           );
