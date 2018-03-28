use strict;
use warnings;
use Module::Build;

print "==> Running the extended test suite may take weeks or years! <==\n";
my $answer = Module::Build->y_n(
    'Do you want to run the extended test suite?', 'n'
);

my $patterns = 't/*.t';

if ($answer)
{
    print "I'm going to run the extended tests.\n";
    $patterns .= ' t/long/*.t';
}
else
{
    print "Skipping extended tests.\n";
}

my $builder = Module::Build->new(
    module_name       => 'MD5::Solve',
    license           => 'perl',
    dist_author       => 'Emily Anne Perlmonger <emmils@example.com>',
    dist_version_from => 'lib/MD5/Solve.pm',
    build_requires    => { 'Test::More' => 0, },
    add_to_cleanup    => ['MD5-Solve-*'],
    test_files        => $patterns,
);

$builder->create_build_script(  );
