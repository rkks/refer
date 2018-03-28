  use Spreadsheet::Read;

  my $file        = 'test.xls';
  my $spreadsheet = ReadData( $file )         or die "Cannot read $file\n";
  my $sheet_count = $spreadsheet->[0]{sheets} or die "No sheets in $file\n";

  for my $sheet_index (1 .. $sheet_count)
  {
      # Skip empty worksheets
      my $sheet = $spreadsheet->[$sheet_index] or next;

      printf( "%s - %02d: [ %-12s ] %3d Cols, %5d Rows\n", $file,
          $sheet_index, $sheet->{label}, $sheet->{maxcol}, $sheet->{maxrow} );

      for my $row ( 1 .. $sheet->{maxrow} )
      {
          print join "\t" => map {
          $sheet->{cell}[$_][$row] // "-" } 1 .. $sheet->{maxcol};
          print "\n";
      }
  }