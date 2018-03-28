  use B::Deparse;
  my $deparse = B::Deparse->new();
  print $deparse->coderef2text($filter);