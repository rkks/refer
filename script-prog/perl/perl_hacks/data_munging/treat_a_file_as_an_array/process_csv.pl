  for my $i ( 0 .. $#csv_lines )
  {
      next unless my ($op, $num) = $csv_lines[ $i ] =~ /^(\w+):(\d+)/;
      next unless my $op_sub     = __PACKAGE__->can( 'op_' . $op );

      my $start                  = $i - $num;
      my $end                    = $i - 1;
      my @lines                  = @csv_lines[ $start .. $end ];
      my @newlines               = $op_sub->( @lines );

      splice @csv_lines, $start, $num + 1, @newlines;
  }