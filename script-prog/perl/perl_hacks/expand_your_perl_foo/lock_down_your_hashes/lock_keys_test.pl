use Test::More tests => 2;

use Hash::Util 'lock_keys';

my %locked   = ( foo => 1, bar => 2 );
my %unlocked = ( foo => 1, bar => 2 );

lock_keys( %locked );

eval {   $locked{fool} = 1 };
eval { $unlocked{fool} = 1 };

is( keys %locked,   2, 'hash with locked keys should disallow unknown key' );
is( keys %unlocked, 3, '... but unlocked hash should not' );
