use YAML 'DumpFile';

sub serialize
{
    my ($object, $file) = @_;
    my %data            = %$object;
    DumpFile( $file, \%data );
}
