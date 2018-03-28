sub make_counter
{
    my ($start, $end, $step) = @_;

    return sub
    {
        return if $start B<==> $end;
        $start += $step;
    };
}
