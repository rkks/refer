use Cwd;

sub change_directory
{
    my $newdir = shift;
    my $curdir = cwd();
    chdir( $newdir );
    return Scope::Guard->new(  sub { chdir $curdir } );
}
