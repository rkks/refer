  use Spreadsheet::ReadSXC qw( read_sxc );

  my $book  = read_sxc( 'test.sxc', { OrderBySheet => 1 } );

  # Sheet by ordinal
  my $sheet = $book->[0];
  my $cell  = $sheet->{data}[2][0];
  print "$cell\n";