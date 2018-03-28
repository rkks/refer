  sub bind_hash
  {
      my ($dbh, $hash_ref, $table, @fields) = @_;

      my $sql = 'SELECT ' . join(', ', @fields) . " FROM $table";
      my $sth = $dbh->prepare( $sql );

      $sth->execute();
      $sth->bind_columns( \@$hash_ref{ @{ $sth->{NAME_lc} } } );

      return sub { $sth->fetch() };
  }