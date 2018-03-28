my $fh;
$fh = eval { open_file( '/dev/null', '<' ) };
warn $@ if $@;

$fh = eval { open_file( '/dev', '>' ) };
warn $@ if $@;
