package Music;
  
use strict;
use warnings;
  
BEGIN
{
    @Cd::ISA     = 'Music';
    @Mp3::ISA    = 'Music';
    @Record::ISA = 'Music';
}
  
sub new
{
    my ($class, $title) = @_;
    bless { title => $title, count => 0 }, $class;
}
  
sub add_play
{
    my $self = shift;
    $self->{count}++;
}
  
sub data
{
    my $self = shift;
    return \%$self;
}
  
sub play {  }
sub type { ref( $_[0] ) }
  
1;
