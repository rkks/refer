#!perl
  
use strict;
use warnings;
  
use lib 'build_lib';
use MyBuild;
  
my $build = MyBuild->new(
    module_name    => 'My::App',
    requires       =>
    {
        'DBI'         => '',
        'DBD::SQLite' => '',
    },
    build_requires =>
    {
        'Test::Simple' => '',
    },
);
  
$build->create_config_file(  );
$build->create_build_script(  );
