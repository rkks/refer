  my $filter = filter(qr/\d/);
  my @lines  = $filter->($file_handle);
