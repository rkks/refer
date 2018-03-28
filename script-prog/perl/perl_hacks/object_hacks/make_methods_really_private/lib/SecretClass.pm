package SecretClass;

my %methods;
use Class::HideMethods \%methods;

sub new            { bless {}, shift }
sub hello :Private { return 'hello'   }
sub goodbye        { return 'goodbye' }

sub public_hello
{
    my $self  = shift;
    my $hello = $methods{hello};
    $self->$hello();
}

1;
