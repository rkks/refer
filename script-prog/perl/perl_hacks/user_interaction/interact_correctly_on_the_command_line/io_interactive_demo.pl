use IO::Interactive qw( is_interactive );

print "> " if is_interactive;
while (my $next_cmd = <>)
{
    chomp $next_cmd;
    process($next_cmd);
    print "> " if is_interactive;
}
