  #!/usr/bin/perl

  use strict;
  use warnings;

  use Time::HiRes 'sleep';

  local $| = 1;

  for ( 1 .. 10000 )
  {
      my $status = $_ % 10 ? 'up' : 'down';
      print "$status\n";
      sleep( 0.1 );
  }