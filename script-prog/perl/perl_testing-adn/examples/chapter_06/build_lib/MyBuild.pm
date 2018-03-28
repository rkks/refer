package MyBuild;
  
use base 'Module::Build';
  
use DBI;
use File::Path;
use Data::Dumper;
use File::Spec::Functions;
  
sub create_config_file
{
    my $self     = shift;
    my $config   = 
    {
        db_type  => $self->prompt( 'Database type ',       'SQLite'   ),
        user     => $self->prompt( 'Database user: ',      'root'     ),
        password => $self->prompt( 'Database password: ',  's3kr1+'   ),
        db_name  => $self->prompt( 'Database name: ',      'app_data' ),
        test_db  => $self->prompt( 'Test database name: ', 'test_db'  ),
    };
    $self->notes( db_config    => $config );
  
    mkpath( catdir( qw( lib My App ) ) );
  
    my $dd       = Data::Dumper->new( [ $config ], [ 'db_config' ] );
    my $path     = catfile(qw( lib My App Config.pm ));
  
    open( my $file, '>', $path ) or die "Cannot write to '$path': $!\n";
  
    printf $file <<'END_HERE', $dd->Dump(  );
package My::App::Config;
  
my $db_config;
%s
  
sub config
{
    my ($self, $key) = @_;
    return $db_config->{$key} if exists $db_config->{$key};
}
  
1;
END_HERE
}
  
sub create_database
{
    my ($self, $dbname) = @_;
    my $config          = $self->notes( 'db_config' );
    my $dbpath          = catfile( 't', $dbname );
  
    local $/            = ";\n";
    local @ARGV         = catfile(qw( sql schema.sql ));
    my @sql             = <>;
  
    my $dbh             = DBI->connect(
        "DBI:$config->{db_type}:dbname=$dbpath",
        @$config{qw( user password )}
    );
    $dbh->do( $_ ) for @sql;
  }
  
sub ACTION_build
{
    my $self   = shift;
    my $config = $self->notes( 'db_config' );
    $self->create_database( $config->{db_name} );
    $self->SUPER::ACTION_build( @_ );
}
  
sub ACTION_test
{
    my $self   = shift;
    my $config = $self->notes( 'db_config' );
    $self->create_database( $config->{test_db} );
    $self->SUPER::ACTION_test( @_ );
}
  
1;
