{
    package Tie::TimeFormatty;
    use Tie::Hash ();
    use base 'Tie::StdHash';
    use Date::Format 'time2str';
    sub FETCH { time2str($_[1], time) }
}

tie my %NowAs, Tie::TimeFormatty;

print "It is now $NowAs{'%Y-%m-%dT%H:%M:%S'}\n";
sleep 3;
print "It is now $NowAs{'%c'}\n";
