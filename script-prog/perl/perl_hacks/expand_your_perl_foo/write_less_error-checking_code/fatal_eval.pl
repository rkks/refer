use Fatal qw( open close );

eval
{
    open( my $fh, '>', '/invalid_directory/invalid_file' );
    print $fh "Hello\n";
    close $fh;
};

die "File error: $!" if $@;
