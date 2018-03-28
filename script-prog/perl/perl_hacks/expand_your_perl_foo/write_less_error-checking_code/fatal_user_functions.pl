package MyCode;

sub succeed { 1 }
sub fail    { 0 }

use Fatal qw( :void succeed fail );

succeed();
fail();

1;
