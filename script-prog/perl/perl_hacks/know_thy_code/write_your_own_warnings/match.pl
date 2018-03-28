sub match
{
    my $op = shift;

    if ( $op->name() eq 'entersub' )
    {
        my $class  = eval { $op->first->sibling->sv->PV };
        my $method = eval { $op->first->sibling->sibling->sv->PV };
        my $loc    = B::Lint->file() . ' line ' . B::Lint->line() . '';

        if ( defined $class )
        {
            no strict 'refs';

            # check strict classes
            if ( not defined %{ $class . '::' } )
            {
                warn "Class $class doesn't exist at $loc.\n";
            }
            # check strict class methods
            elsif ( not $class->can($method) )
            {
                warn "Class $class can't do method $method at $loc.\n";
            }
        }
        elsif ( not grep { $_->can($method) } qw( Foo::Bar Foo::Baz ) )
        {
            warn "Object can't do method $method at $loc.\n";
        }
    }
}
