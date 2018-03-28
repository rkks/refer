use Test::More tests => 1;
use Test::Deep;
  
my @a = ( 4, 89, 2, 7, 1 );
  
cmp_bag( \@a, [ 1, 2, 4, 7, 89 ] );
