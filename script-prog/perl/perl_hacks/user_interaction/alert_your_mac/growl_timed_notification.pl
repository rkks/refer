my %seconds_per =
(
    's'   => 1,
    'm'   => 60,
    'h'   => 60*60,
);

my ( $period, @message ) = @ARGV;
my ( $number, $unit )    = ( $period =~ m/^([\.\d]+)(.*)$/ )
      or die "usage: ga number[smh] message\n";
$unit ||= 's';

my $growl_time = $number * $seconds_per{$unit};

my $pid        = fork;
die "fork failed ($!)\n" unless defined $pid;

unless ( $pid )
{
    require Mac::Growl;
    sleep $growl_time;

    Mac::Growl::PostNotification(
        'growlalert', # application name
        'alert',      # type of notification
        "@message",   # title
        "",           # no description
        1,            # notification is sticky
    );
}
