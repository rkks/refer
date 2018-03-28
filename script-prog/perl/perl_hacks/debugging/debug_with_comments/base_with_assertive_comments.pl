  use Smart::Comments;

  my $call       = "26, 17, 22, hut!";

  my @play_calls = split /\s*,?\s*/, $call;

  #### require: @play_calls == 4