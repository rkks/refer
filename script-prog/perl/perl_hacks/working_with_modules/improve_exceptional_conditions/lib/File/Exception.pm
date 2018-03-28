package File::Exception;

use SUPER;
use Exception::Class;

use base 'Exception::Class::Base';

sub Fields
{
    my $self = shift;
    return super(), qw( file mode );
}

sub file { $_[0]->{file} }
sub mode { $_[0]->{mode} }

sub full_message
{
    my $self = shift;
    my $msg  = $self->message();

    my $file = $self->file();
    my $mode = $self->mode();

    return "Exception '$msg' when opening file '$file' with mode '$mode'";
}

1;
