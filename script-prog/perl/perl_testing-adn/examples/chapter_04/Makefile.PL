use strict;
use warnings;
use ExtUtils::MakeMaker qw( WriteMakefile prompt );

my %config = (
    NAME          => 'MD5::Solve',
    AUTHOR        => 'Emily Anne Perlmonger <emmils@example.com>',
    VERSION_FROM  => 'lib/MD5/Solve.pm',
    ABSTRACT_FROM => 'lib/MD5/Solve.pm',
    PREREQ_PM     => { 'Test::More' => 0, },
    dist          => { COMPRESS => 'gzip -9f', SUFFIX => 'gz', },
    clean => { FILES => 'MD5-Solve-*' },
);

my @patterns = qw( t/*.t );

print "==> Running the extended test suite may take weeks or years! <==\n";
my $answer = prompt( 'Do you want to run the extended test suite?', 'no' );

if ( $answer =~ m/^y/i ) 
{
    print "I'm going to run the extended tests.\n";
    push @patterns, 't/long/*.t';
}
else 
{
    print "Skipping extended tests.\n";
}

$config{test} = { TESTS => join ' ', map { glob } @patterns };

WriteMakefile(%config);
