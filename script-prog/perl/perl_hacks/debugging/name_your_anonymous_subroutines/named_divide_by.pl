  sub divide_by
  {
      my $numerator = shift;
      my $name      = (caller(0))[3];
      return sub
      {
          local *__ANON__ = "__ANON__$name";
          my $denominator = shift;
          croak "Denominator must not be zero!" unless $denominator;
          return $numerator / $denominator;
      };
  }