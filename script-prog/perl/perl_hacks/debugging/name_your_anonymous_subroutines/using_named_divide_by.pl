  use Carp;
  
  sub divide_by
  {
      my ($name, $numerator) = @_;
      return sub
      {
          local *__ANON__ = "__ANON__$name";
          my $denominator = shift;
          croak "Denominator must not be zero!" unless $denominator;
          return $numerator / $denominator;
      };
  }
  
  my $three_divided_by = divide_by( 'divide_by_three', 3 );
  my $answer           = $three_divided_by->(0);