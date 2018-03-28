package Person;
# Class for storing data about a person
#person5.pm
use warnings;
use strict;
use Carp;

sub new {
   my $class = shift; 
   my $self = {@_}; 
   bless($self, $class); 
   return $self; 
}

sub surname {
   my $self = shift;
   unless (ref $self) {
      croak "Should call surname() with an object, not a class";
   }
   return $self->{surname}
}

1;
