# create a new scope for the lexicals
{
    package InsideOut::User;

    use Scalar::Util 'refaddr';

    # lexicals used to hold instance data
    my %names;
    my %addresses;

    sub new
    {
        my ($class, $data) = @_;

        # bless a new scalar to get this object's id
        bless \(my $self), $class;

        # store the instance data
        my $id             = refaddr( $self );
        $names{     $id }  = $data->{name};
        $addresses{ $id }  = $data->{address};

        return $self;
    }

    # accessors, as $self->{name} and $self->{address} don't work
    sub get_name
    {
        my $self = shift;
        return $names{ refaddr( $self ) };
    }

    sub get_address
    {
        my $self = shift;
        return $addresses{ refaddr( $self ) };
    }

    # many people forget this part
    sub DESTROY
    {
        my $self = shift;
        my $id   = refaddr( $self );
        delete $names{     $id };
        delete $addresses{ $id };
    }
}

1;
