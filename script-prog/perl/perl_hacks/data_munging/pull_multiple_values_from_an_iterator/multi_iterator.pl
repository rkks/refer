  use Want 'howmany';

  sub multi_iterator
  {
      my ($iterator) = @_;

      return sub
      {
          my $context = wantarray();

          return               unless defined $context;
          return $iterator->() unless         $context;
          return map { $iterator->() } 1 .. howmany();
      };
  }