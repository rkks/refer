sub slurp
{
    my ($file) = @_;
    open my $fh, '<', $file or croak "Couldn't open '$file'";
    local $/;
    return <$fh>;
}

# and thereafter:

my $contents = slurp $filename;
