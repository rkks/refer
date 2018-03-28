  use Spreadsheet::ParseExcel;

  my $workbook  = Spreadsheet::ParseExcel::Workbook->Parse( 'test.xls' );
  my $worksheet = $workbook->{Worksheet}[0];
  my $cell      = $worksheet->{Cells}[2][0];
  print $cell->Value(), "\n";