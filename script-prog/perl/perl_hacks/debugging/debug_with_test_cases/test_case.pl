  use Test::More tests => 4;

  my @elements =
  (
      [ 2, 2 ], [ 2, 1 ], [ 2, 0 ],
      [ 1, 0 ], [ 1, 1 ], [ 1, 2 ],
  );
  my @sorted   = sort { $a->[0] <=> $b->[0] } @elements;

  is( $sorted[0][0], 1, 'numeric sort should put 1 before 2'     );
  is( $sorted[0][1], 0, '... keeping stability of original list' );
  is( $sorted[2][1], 2, '... through all elements'               );
  is( $sorted[3][1], 2, '... not accidentally sorting them'      );