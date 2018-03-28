{
    package TimeVar_YMDhms;

    use Tie::Scalar ();
    use base 'Tie::StdScalar';
    use Date::Format 'time2str';

    sub FETCH { time2str('%Y-%m-%dT%H:%M:%S', time) }
}

tie my $TIME, TimeVar_YMDhms;

print "It is now $TIME\n";
sleep 3;
print "It is now $TIME\n";
