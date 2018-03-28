use Test::More;

eval 'use Test::Pod 1.00';
plan( skip_all => 'Test::Pod 1.00 required for testing POD' ) if $@;


all_pod_files_ok(
    'lib/Sports/NASCAR/Car.pm',
    'lib/Sports/NASCAR/Driver.pm',
    'lib/Sports/NASCAR/Team.pm'
);
