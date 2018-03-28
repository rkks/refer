package Attribute::Docstring;

use strict;
use warnings;

use Scalar::Util 'blessed';
use Attribute::Handlers;

my %doc;

sub UNIVERSAL::Doc :ATTR
{
    my ($package, $symbol, $referent, $attr, $data, $phase) = @_;
    return if $symbol eq 'LEXICAL';

    my $name                = *{$symbol}{NAME};
    $doc{ $package }{$name} = $data;
}

sub UNIVERSAL::doc
{
    my ($self, $name) = @_;
    my $package       = blessed( $self ) || $self;

    return unless exists $doc{ $package }{ $name };
    return               $doc{ $package }{ $name };
}

1;
