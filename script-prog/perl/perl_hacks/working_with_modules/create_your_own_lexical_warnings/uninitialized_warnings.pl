use warnings;

# ... more code here...

sub say
{
    no warnings 'uninitialized';
    print join( $,, @_ ), "\n";
}
