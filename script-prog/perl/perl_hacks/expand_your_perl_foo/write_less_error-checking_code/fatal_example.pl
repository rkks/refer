use Fatal qw( open close );

open( my $fh, '>', '/invalid_directory/invalid_file' );
print $fh "Hello\n";
close $fh;
