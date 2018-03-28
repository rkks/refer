package Graphics::Drawable;
{
    use Class::Std;

    my %coords_of     :ATTR( :get<coords>   :init_arg<coords>   );
    my %velocities_of :ATTR( :get<velocity> :init_arg<velocity> );
    my %shapes_of     :ATTR( :get<shape>    :init_arg<shape>    );

    sub get_serializable_data
    {
        my $self  = shift;

        my %data;

        for my $attribute (qw( coords velocity shape ))
        {
            my $method = 'get_' . $attribute;
            $data{ $attribute } = $self->$method();
        }

        return \%data;
    }
}

1;
