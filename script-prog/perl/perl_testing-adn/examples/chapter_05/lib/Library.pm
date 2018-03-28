package Library;
  
use strict;
use warnings;
  
use Carp 'croak';
use File::Spec::Functions qw( catdir catfile );
  
sub new
{
    my ($class, $path) = @_;
    bless \$path, $class;
}
  
sub path
{
    my $self = shift;
    return $$self;
}
  
sub load
{
    my ($self, $type, $id) = @_;
    my $directory          = $self->find_dir( $type );
    my $data               = $self->read_file( $directory, $id );
    bless $data, $type;
}
  
sub save
{
    my ($self, $object, $id) = @_;
    my $directory            = $self->find_dir( $object->type(  ) );
    $self->save_file( $directory, $id, $object->data(  ) );
}
  
sub find_dir
{
    my ($self, $type) = @_;
    my $path          = $self->path(  );
    my $directory     = catdir( $path, $type );
    croak( "Unknown directory '$directory'" ) unless -d $directory;
    return $directory;
}
  
sub read_file {  }
sub save_file {  }
  
1;
