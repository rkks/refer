  use FileReadBackwards;

  my $bw = File::ReadBackwards->new( 'status.log' )
      or die "Cannot read 'status.log': $!\n";

  exit( 0 ) if $bw->readline() =~ /up/;

  # panic() ...