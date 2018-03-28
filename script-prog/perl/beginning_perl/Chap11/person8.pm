package Person;
# Class for storing data about a person
#person8.pm
use warnings;
use strict;
use Carp;

my @Everyone;

# Constructor and initialisation
sub new {
   my $class = shift; 
   my $self = {@_}; 
   bless($self, $class); 
   $self->_init;
   return $self; 
}

sub _init {
   my $self = shift;
   push @Everyone, $self;
   carp "New object created";
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
sub headcount { scalar @Everyone }
sub everyone  { @Everyone        }

1;
