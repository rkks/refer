use Scalar::Util qw( openhandle );

sub is_interactive
{
    # Not interactive if output is not to terminal...
    return 0 if not -t select;

    # If *ARGV is opened, we're interactive if...
    if (openhandle *ARGV)
    {
        # ...it's currently opened to the magic '-' file
        #    and the standard input is interactive...
        return -t *STDIN if defined $ARGV && $ARGV eq '-';

        # ...or it's at end-of-file and the next file
        #    is the magic '-' file...
        return @ARGV>0 && $ARGV[0] eq '-' && -t *STDIN if eof *ARGV;

        # ...or it's directly attached to the terminal
        return -t *ARGV;
    }

    # If *ARGV isn't opened, it will be interactive if *STDIN is
    # attached to a terminal and either there are no files specified
    # on the command line or if there are files and the first is the
    # magic '-' file...
    else
    {
        return -t *STDIN && (@ARGV==0 || $ARGV[0] eq '-');
    }
}
