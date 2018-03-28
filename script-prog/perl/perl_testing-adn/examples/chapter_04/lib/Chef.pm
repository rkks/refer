package Chef;

use base 'Exporter';

use strict;
use warnings;

our $VERSION = '1.0';
our @EXPORT  = qw( slice dice fricassee );

sub slice
{
    my $ingredient = shift;
    print "Slicing $ingredient...\n";
}

sub dice
{
    my $ingredient = shift;
    print "Dicing $ingredient...\n";
}

sub fricassee
{
    my $ingredient = shift;
    print "Fricasseeing $ingredient...\n";
}

sub boil
{
    my $ingredient = shift;
    print "Boiling $ingredient...\n";
}

1;
