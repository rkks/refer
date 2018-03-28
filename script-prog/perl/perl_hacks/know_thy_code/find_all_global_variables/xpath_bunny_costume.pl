use B::XPath;

my $node = B::XPath->fetch_root( \&wear_bunny_costume );

for my $global ( $node->match( '//gvsv' ) )
{
    my $location = $global->find_nextstate();
    printf( "Global %s found at %s:%d\n",
    $global->NAME(), $location->file(), $location->line() );
}
