use Contextual::Return;
use Time::HiRes qw( sleep time );      # Allow subsecond timing

# Subroutine returns an active timer value...
sub timer
{
    my $start = time;                  # Set initial start time

    return VALUE                       # Return an active value that...
    {                     
        my $elapsed = time - $start;   #    1. computes elapsed time
        $start      = time;            #    2. resets start time
        return $elapsed;               #    3. returns elapsed time
    }
}

# Create an active value...
my $process_timer = timer();

# Use active value...
while (1)
{
    do_some_long_process();
    print "Process took $process_timer seconds\n";
}
