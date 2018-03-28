package UserFactory;

use User;
use UserProxy;

my $count    = 0;

sub create
{
   my $self  = shift;
   my $class = $count++ % 100 ? 'User' : 'UserProxy';
   return $class->new( id => $count, @_ );
}

1;
