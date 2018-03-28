print "The clock's already ticking\n"
    if stopwatch();                              # treat as a boolean
stopwatch(1);                                    # start
do_stuff();
stopwatch(0);                                    # stop
print "Did stuff in ", stopwatch(), "\n";        # report as string

stopwatch(undef);                                # reset
stopwatch(1);                                    # start
do_more_stuff();
print "Did more stuff in ", stopwatch(0), "\n";  # stop and report

print "Sorry for the delay\n"
    if stopwatch() > 5;                          # treat as number
