{
    package Tie::ScalarFnParams;
    sub TIESCALAR
    {
        my($class, $fn, @params) = @_;
        return bless sub { $fn->(@params) }, $class;
    }

    sub FETCH { return shift()->() }
    sub STORE { return } # called for $var = somevalue;
}

use Date::Format 'time2str';

tie my $TIME, Tie::ScalarFnParams,
 # And now any function and optional parameter(s):
   sub { time2str(shift, time) }, '%Y-%m-%dT%H:%M:%S';

print "It is now $TIME\n";
sleep 3;
print "It is now $TIME\n";
