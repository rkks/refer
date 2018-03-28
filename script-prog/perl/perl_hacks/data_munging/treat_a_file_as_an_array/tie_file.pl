  use Tie::File;

  tie my @csv_lines, 'Tie::File', 'big_file.csv'
      or die "Cannot open big_file.csv: !$\n";