#!perl

use strict;
use warnings;

print "> ";

while (<>)
{
    chomp;
    last unless $_;

    my ($command, @args) = split( /\s+/, $_ );

    my $sub;
    unless ($sub = __PACKAGE__->can( $command ))
    {
        print "Unknown command '$command'\n> ";
        next;
    }

    $sub->(@args);
    print "> ";
}

sub add
{
    my $result = 0;

    $result += $_ for @_;
    print join(" + " , @_ ), " = $result\n";
}

sub subtract
{
    my $result = shift;

    print join(" - " , $result, @_ );

    $result -= $_ for @_;
    print " = $result\n";
}
