  use Text::CSV_XS;

  my $csv = Text::CSV_XS->new();
  @ARGV   = ( 'test.csv' );

  # Skip the first two rows
  <> for 1 .. 2;
  my $row = <>;

  $csv->parse( $row );
  my $col = ( $csv->fields )[0];
  print "$col\n";