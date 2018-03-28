  my @ranked;

  for my $rank ( 1 .. 10 )
  {
      $ranked[$rank] = get_player_by_rank( $rank );
  }