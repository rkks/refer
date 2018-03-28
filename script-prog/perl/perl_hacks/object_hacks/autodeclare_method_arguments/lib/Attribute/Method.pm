package Attribute::Method;

use strict;
use warnings;

use B::Deparse;
use Attribute::Handlers;

my $deparse = B::Deparse->new();

sub import
{
    my ( $class, @vars ) = @_;
    my $package          = caller();

    my %references       =
    (
        '$' => \undef,
        '@' => [],
        '%' => {},
    );

    push @vars, '$self';

    for my $var (@vars)
    {
        my $reftype                 = substr( $var, 0, 1, '' );

        no strict 'refs';
        *{ $package . '::' . $var } = $references{$reftype};
    }
}

sub UNIVERSAL::Method :ATTR(RAWDATA)
{
    my ($package, $symbol, $referent, undef, $arglist) = @_;

    my $code                 = $deparse->coderef2text( $referent );
    $code                    =~ s/{/sub {\nmy (\$self, $arglist) = \@_;\n/;

    no warnings 'redefine';
    *$symbol                 = eval "package $package; $code";
}

1;
