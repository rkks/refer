#!perl

use strict;
use warnings;

use Fatal qw( mkdir open close );
use File::Spec::Functions;

mkdir( 'music_history' ) unless -d 'music_history';

for my $subdir (qw( handel vivaldi telemann ))
{
    my $dir = catdir( 'music_history', $subdir );
    mkdir( $dir ) unless -d $dir;
}

sleep 1;

for my $period (qw( baroque classical ))
{
    open( my $fh, '>', catfile( 'music_history', $period ));
    print $fh '18th century';
    close $fh;
    sleep 1;
}
