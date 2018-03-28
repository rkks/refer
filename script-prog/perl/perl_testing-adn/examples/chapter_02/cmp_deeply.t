use Test::More tests => 1;
use Test::Deep;
  
my $points =
[
    { x => 50, y =>  75 },
    { x => 19, y => -29 },
];
  
my $is_integer = re('^-?\d+$');
  
cmp_deeply( $points,
  array_each(
    {
      x => $is_integer,
      y => $is_integer,
    }
  ),
        'both sets of points should be integers' );
