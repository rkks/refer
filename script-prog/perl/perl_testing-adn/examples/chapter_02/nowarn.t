use Test::More tests => 3;
use Test::NoWarnings;
  
sub add_positives {
    my ( $l, $r ) = @_;
    warn "first argument ($l) was negative"  if $l < 0;
    warn "second argument ($r) was negative" if $r < 0;
    return $l + $r;
}
  
is( add_positives( 4,  6 ), 10 );
is( add_positives( 8, -3 ),  5 );
