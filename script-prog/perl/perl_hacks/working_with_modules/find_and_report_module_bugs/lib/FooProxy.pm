package FooProxy;

sub new
{ 
    my $class = shift;
    my $foo   = Foo->new( @_ );
    bless \$foo, $class;
  }

sub can
{ 
    my $self = shift;
    return $$self->can( @_ );
}

1;
