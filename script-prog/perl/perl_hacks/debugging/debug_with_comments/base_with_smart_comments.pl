  # make '###' magical...
  use Smart::Comments;

  my $call       = "26, 17, 22, hut!";

  ### $call

  my @play_calls = split /\s*,?\s*/, $call;

  ### @play_calls