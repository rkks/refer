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

# Utility methods
sub fullname {
   my $self = shift;
   return $self->forename." ".$self->surname;
}

sub printletter {
   my $self    = shift;
   my $name    = $self->fullname;
   my $address = $self->address;
   my $forename= $self->forename;
   my $body    = shift;
   my @date    = (localtime)[3,4,5];
   $date[1]++;      # Months start at 0! Add one to humanise!
   $date[2]+=1900;  # Add 1900 to get current year.
   my $date    = join "/", @date;

   print <<EOF;
$name
$address

$date

Dear $forename,

$body

Yours faithfully,
EOF
   return $self;
}


1;
