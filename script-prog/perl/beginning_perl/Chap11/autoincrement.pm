package Autoincrement;
#autoincrement.pm
use warnings;
use strict;

sub TIESCALAR {
   my $class = shift; # No parameters
   my $realdata = 0;
   return bless \$realdata, $class;
}

sub FETCH {
   my $self = shift;
   return $$self++;
}

sub STORE {
   my $self  = shift;
   my $value = shift;
   warn "Hi, you said $value\n";
   $$self = 0;
}


1;
