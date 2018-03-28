if (wantarray)                # wantarray true      --> list context
{
    return @some_list;
}
elsif (defined wantarray)     # wantarray defined   --> scalar context
{
    return $some_scalar;
}
else                          # wantarray undefined --> void context
{
    do_something();
    return;
}
