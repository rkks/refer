  sub counter
  {
      my ($from, $to, $step)  = @_;
      $step                 ||= 1;

      return sub
      {
          return if $from > $to;
          my $value       = $from;
          $from          += $step;
          return $value;
      };

  }