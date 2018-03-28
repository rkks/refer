package Model;

sub new
{
    my ($class, %args) = @_;
    bless \%args, $class;
}

sub get_data
{
    my $self = shift;
    my %data = map { $_ => $self->{$_} } qw( name occupation age );
    return \%data;
}

1;
