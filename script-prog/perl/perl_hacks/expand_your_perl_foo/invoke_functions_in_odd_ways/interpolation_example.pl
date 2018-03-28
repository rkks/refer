use Date::Format 'time2str';
use Interpolation  NowAs => sub { time2str($_[0],time) };

print "It is now $NowAs{'%Y-%m-%dT%H:%M:%S'}\n";
sleep 3;
print "It is now $NowAs{'%c'}\n";
