use Parser;

my $parser = eval { Parser->parse( 'some example text' ) };
die "Bad input to parser: $@\n" if $@;
