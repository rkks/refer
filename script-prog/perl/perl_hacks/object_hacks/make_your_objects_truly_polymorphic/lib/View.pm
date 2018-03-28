package View;

use Class::Trait 'base';

package TextView;

use base 'View';

sub render
{
    my $self = shift;
    printf( "My name is %s.  I am an %s and I am %d years old.\n",
        @{ $self->get_data() }{qw( name occupation age )} );
}

package YAMLView;

use YAML;
use base 'View';

sub render
{
    my $self = shift;
    print Dump $self->get_data();
}

1;
