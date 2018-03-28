package Module::Build::FilterTests;

use base 'Module::Build';

use SUPER;
use File::Spec::Functions;

sub ACTION_disttest
{
    local $ENV{PERL_RUN_ALL_TESTS} = 1;
    super();
}

sub find_test_files
{
    my $self  = shift;
    my $tests = super();
  
    return $tests unless $ENV{PERL_RUN_ALL_TESTS};

    my $test_pattern = catfile(qw( t developer *.t ) );
    push @$tests, <$test_pattern>;
    return $tests;
}

1;
