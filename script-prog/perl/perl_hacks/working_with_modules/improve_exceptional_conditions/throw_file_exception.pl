use File::Exception;

sub open_file
{
    my ($name, $mode) = @_;

    open my $fh, $mode, $name or
        File::Exception->throw( file => $name, mode => $mode, error => $! );

    return $fh;
}
