package Person;
# Class for storing data about a person
#person3.pm
use warnings;
use strict;

sub new {
   my $class = shift;
   my $self = {@_};
   bless($self, $class);
   return $self; 
}

1;
