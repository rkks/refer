package Jukebox;
  
use strict;
use warnings;
  
sub new
{
    my ($class, $library) = @_;
    bless { library => $library }, $class;
}
  
sub library
{
    my $self = shift;
    return $self->{library};
}
  
sub play_music
{
    my ($self, $medium, $title) = @_;
  
    my $class                   = ucfirst( lc( $medium ) );
    my $library                 = $self->library(  );
    my $music                   = $library->load( $class, $title );
    return unless $music;
  
    $music->play(  );
    $music->add_play(  );
  
    $library->save( $music, $title, $music );
}
  
1;
