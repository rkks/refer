#!perl

use strict;
use warnings;

use Test::More tests => 5;
use Test::Exception;

use File::Spec::Functions;

ok( require( 'filefilter.pl' ), 'loaded file okay' ) or exit;

throws_ok { main() } qr/Usage:/,
    'main() should give a usage error without any arguments';

throws_ok { main( 'bad command' ) } qr/Unknown command 'bad command'/,
    '... or with a bad command given';

my @directories =
(
    'music_history',
    map { catdir( 'music_history', $_ ) } qw( handel vivaldi telemann )
);

my @files = map { catfile( 'music_history', $_ ) } qw( baroque classical );

is_deeply( [ cmd_dirs( @directories, @files ) ], \@directories,
    'dirs command should return only directories' );

is( cmd_latest( @files ), catfile(qw( music_history classical )),
    'latest command should return most recently modified file' );
