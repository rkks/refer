  # assume you already have $dbh connected

  my %user;

  my $user_fetch = bind_hash( $dbh, \%user, qw( users name dob shoe_size ) );

  while ($user_fetch->())
  {
      print "$user{name}, born on $user{dob}, wears a size " .
            "$user{shoe_size} shoe\n";
  }