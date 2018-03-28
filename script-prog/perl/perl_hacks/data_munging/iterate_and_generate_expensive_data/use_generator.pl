  my $next_address   = create_generator( '192.168.1.0/8', '10.0.0.0/16' );

  while (my $address = $next_address->())
  {
      # try to communicate with machine at $address
  }