use IO::Interactive qw( interactive );

print {interactive} "> ";
while (my $next_cmd = <>)
{
    chomp $next_cmd;
    process($next_cmd);
    print {interactive} "> ";
}
