package Person;
# Class for storing data about a person
#person2.pm
use warnings;
use strict;

sub new {
   my $self = {};
   bless ($self, "Person");
   return $self;
}

1;
