use Time::HiRes qw( time );
use Contextual::Return;

my $elapsed       = 0;
my $started_at    = 0;
my $is_running    = 0;

# Convert elapsed seconds to HH::MM::SS string...
sub _HMS
{
    my ($elapsed) = @_;
    my $hours     = int($elapsed / 3600);
    my $mins      = int($elapsed / 60 % 60);
    my $secs      = int($elapsed) % 60;
    return sprintf "%02d:%02d:%02d", $hours, $mins, $secs;
}

sub stopwatch
{
    my ($run)     = @_;

    # Update elapsed time...
    my $now       =  time();
    $elapsed     += $now - $started_at if $is_running;
    $started_at   =  $now;

    # Defined arg turns stopwatch on/off, undef arg resets it...
    $is_running   =  $run if @_;
    $elapsed      =  0 if @_ && !defined $run;

    # Handle different scalar contexts...
    return
         NUM { $elapsed         }
         STR { _HMS( $elapsed ) }
        BOOL { $is_running      }
}
