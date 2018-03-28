  use SQL::Library;

  my $library = SQL::Library->new({ lib => 'daily_reports.sql' });

  for my $query ( $library->elements() )
  {
      my $sth = $dbh->prepare( $query );
      my %columns;

      $sth->bind_columns( \@columns{ @{ $sth->{NAME_lc} } } );
      $sth->execute();

      process_report( \%columns );
  }