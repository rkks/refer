#!perl

use Module::Build;

my $build_pkg =
    eval { require Apache::TestMB } ? 'Apache::TestMB' : 'Module::Build';

my $build = $build_pkg->new(
    module_name  => 'Thumbnail',
    dist_version => 0.01,
    license      => 'perl',
    requires     => {
        'Apache::Test' => 1.12,
        'Imager'       => 0.40,
    },
);

$build->create_build_script();
