  use Carp;
  
  sub divide_by
  {
      my $numerator = shift;
      return sub
      {
          my $denominator = shift;
          croak "Denominator must not be zero!" unless $denominator;
          return $numerator / $denominator;
      };
  }
  
  my $seven_divided_by = divide_by(7);
  my $answer           = $seven_divided_by->(0);