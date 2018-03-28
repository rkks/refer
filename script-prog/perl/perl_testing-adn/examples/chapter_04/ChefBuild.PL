use Module::Build::TestReporter;

my $build = Module::Build::TestReporter->new(
      module_name       => 'Chef',
      license           => 'perl',
      report_file       => 'chef_failures.txt',
      report_address    => 'chef-failures@example.com',
      dist_version_from => 'lib/Chef.pm',
);

$build->create_build_script(  );
