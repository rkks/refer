print "> ";
while (my $next_cmd = <>)
{
    chomp $next_cmd;
    process($next_cmd);
    print "> ";
}
