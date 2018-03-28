package Module::Build::Configurator;

use strict;
use warnings;

use base 'Module::Build';

use SUPER;
use File::Path;
use Data::Dumper;
use File::Spec::Functions;

sub new
{
    my ($class, %args) = @_;
    my $self           = super();
    my $config         = $self->notes( 'config_data' ) || {};

    for my $question ( @{ $args{config_questions} } )
    {
        my ($q, $name, $default) = map { defined $_ ? $_ : '' } @$question;
        $config->{$name}         = $self->prompt( $q, $default );
    }

    $self->notes( 'config_module', $args{config_module} );
    $self->notes( 'config_data',   $config );
    return $self;
}

sub ACTION_build
{
    $_[0]->write_config();
    super();
}

sub write_config
{
    my $self      = shift;
    my $file      = $self->notes( 'config_module' );
    my $data      = $self->notes( 'config_data' );
    my $dump      = Data::Dumper->new( [ $data ], [ 'config_data' ] )->Dump();
    my $file_path = catfile( 'lib', split( /::/, $file . '.pm' ) );

    my $path      = ( splitpath( $file_path ) )[1];
    mkpath( $path ) unless -d $path;

    my $package   = <<END_MODULE;
    package $file;

    my $dump

    sub get_value
    {
        my (\$class, \$key) = \@_;

        return unless exists \$config_data->{ \$key };
        return               \$config_data->{ \$key };
    }
    
    1;
END_MODULE

    $package =~ s/^\t//gm;

    open( my $fh, '>', $file_path )
        or die "Cannot write config file '$path': $!\n";
    print $fh $package;
    close $fh;
}

1;
