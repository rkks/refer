package Person;
# Class for storing data about a person
#person6.pm
use warnings;
use strict;
use Carp;

my $Population = 0;

sub new {
   my $class = shift; 
   my $self = {@_}; 
   bless($self, $class); 
   $Population++;
   return $self; 
}

# Object accessor methods
sub address  { $_[0]->{address }=$_[1] if defined $_[1]; $_[0]->{address } }
sub surname  { $_[0]->{surname }=$_[1] if defined $_[1]; $_[0]->{surname } } 
sub forename { $_[0]->{forename}=$_[1] if defined $_[1]; $_[0]->{forename} }
sub phone_no { $_[0]->{phone_no}=$_[1] if defined $_[1]; $_[0]->{phone_no} } 
sub occupation  {
   $_[0]->{occupation}=$_[1] if defined $_[1]; $_[0]->{occupation} 
}

# Class accessor methods
sub headcount { $Population }

1;
