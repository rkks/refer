  use Smart::Comments;

  my $call       = "26, 17, 22, hut!";

  ### input: $call

  my @play_calls = split /\s*,?\s*/, $call;

  ### split to: @play_calls