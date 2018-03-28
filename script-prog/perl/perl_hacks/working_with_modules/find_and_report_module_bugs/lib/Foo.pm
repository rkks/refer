package Foo;
use UNIVERSAL::isa;

sub new
{ 
    my $class = shift;
    bless \my $foo, $class;
}

sub isa
{ 
    1;
}

1;
