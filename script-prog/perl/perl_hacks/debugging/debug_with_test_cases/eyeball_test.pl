  my @elements =
  (
      [ 2, 2 ], [ 2, 1 ], [ 2, 0 ],
      [ 1, 0 ], [ 1, 1 ], [ 1, 2 ],
  );

  my @sorted   = sort { $a->[0] <=> $b->[0] } @elements;

  local $"     = ', ';
  print "[ @$_ ]\n" for @sorted;