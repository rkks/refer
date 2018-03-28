  use Spreadsheet::Read;

  my $ref  = ReadData( 'test.xls' );
  my $fval = $ref->[1]{A3};
  my $uval = $ref->[1]{cell}[1][3];