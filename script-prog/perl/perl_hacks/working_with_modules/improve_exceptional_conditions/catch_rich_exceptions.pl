$fh = eval { open_file( '/dev', '>' ) };

if (my $error = $@)
{
    warn sprintf "Tried to open %s '%s' as user %s at %s: %s\n",
        $error->mode(), $error->file(), $error->uid(),
        scalar( localtime( $error->time() ) ),
        $error->error();
}
