  use Smart::Comments '####';   # Only ####... comments are "smart"
                                # Any ###... comments are ignored

  my $call       = "26, 17, 22, hut!";

  ### $call

  my @play_calls = split /\s*,?\s*/, $call;

  ### @play_calls

  #### require: @play_calls == 4