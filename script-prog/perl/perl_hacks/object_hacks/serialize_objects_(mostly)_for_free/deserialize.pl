use YAML 'LoadFile';

sub deserialize
{
    my ($class, $file) = @_;
    my $data           = LoadFile( $file );
    return $class->new( $data );
}
