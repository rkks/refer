use Test::More tests => 6;

my $sc = SecretClass->new();
isa_ok( $sc, 'SecretClass' );

ok( ! $sc->can( 'hello' ),        'hello() should be hidden'               );
ok( $sc->can( 'public_hello' ),   'public_hello() should be available'     );
is( $sc->public_hello(), 'hello', '... and should be able to call hello()' );
ok( $sc->can( 'goodbye' ),        'goodbye() should be available'          );
is( $sc->goodbye(), 'goodbye',    '... and should be callable'             );
