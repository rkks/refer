  sub convert_to_base
  {
      my $converted  = '';

      while ($_[1] > 0)
      {
          $converted = $NUMERAL_FOR[$_[1] % $_[0]] . $converted;
          $_[1]      = int( $_[1] / $_[0]);
      }

      return $converted;
  }
