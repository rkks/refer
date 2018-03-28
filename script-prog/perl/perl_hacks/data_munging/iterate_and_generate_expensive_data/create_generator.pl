  use Net::Netmask;

  sub create_generator
  {
      my @netmasks;

      for my $block (@_)
      {
          push @netmasks, Net::Netmask->new( $block );
      }

      my $nth = 1;

      return sub
      {
          return unless @netmasks;
          my $next_ip = $netmasks[0]->nth( $nth++ );

          if ( $next_ip eq $netmasks[0]->last() )
          {
              shift @netmasks;
              $nth = 1;
          }

          return $next_ip;
      }
  }