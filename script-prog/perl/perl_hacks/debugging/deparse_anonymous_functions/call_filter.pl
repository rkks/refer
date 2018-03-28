  my $filter = filter(/\d/);
  my @lines  = $filter->($file_handle);