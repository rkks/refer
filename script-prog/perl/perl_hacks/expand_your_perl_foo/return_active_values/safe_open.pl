use Contextual::Return;

sub safe_open
{
    my ($mode, $filename) = @_;
    my $user_has_tested   = 0;

    # Open a filehandle and remember where it was opened...
    open my($filehandle), $mode, $filename;
    my $where = sprintf("'%s' line %s", (caller)[1,2]);

    # Return an active value that's only usable after it's been tested...
    return (
        BOOL
        {
            $user_has_tested = 1;
            return defined $filehandle;
        }
        DEFAULT
        {
            croak "Used untested filehandle (opened at $where)"
                unless $user_has_tested;
            return $filehandle;
        }
    )
}
