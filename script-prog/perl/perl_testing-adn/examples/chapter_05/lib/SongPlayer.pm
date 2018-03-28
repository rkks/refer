package SongPlayer;
  
use strict;
use warnings;
  
use Carp;
  
sub new
{
    my ($class, %args) = @_;
    bless \%args, $class;
}
  
sub song
{
    my $self      = shift;
    $self->{song} = shift if @_;
    $self->{song};
}
  
sub player
{
    my $self        = shift;
    $self->{player} = shift if @_;
    $self->{player};
}
  
sub play
{
    my $self   = shift;
    my $player = $self->player(  );
    my $song   = $self->song(  );
  
    system( $player, $song ) == 0 or 
        croak( "Couldn't launch $player for $song: $!\n" );
}
  
1;
