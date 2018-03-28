use P5NCI::Library;

my $lib = P5NCI::Library->new( library => 'm' );
$lib->install_function( 'cbrt', 'dd' );

print cbrt( 27 ), "\n";
print cbrt( 31 ), "\n";
