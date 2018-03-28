  sub filter
  {
      my ($filter) = @_;
 
      if ('Regexp' eq ref $filter)
      {
          return sub
          {
              my $fh = shift;
              return grep { /$filter/ } <$fh>;
          };
      }
      else
      {
          return sub
          {
              my $fh = shift;
              return grep { 0 <= index $_, $filter } <$fh>;
          };
      }
  }