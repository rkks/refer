package MyCode;

use base 'Exporter';
our @EXPORT = qw( succeed fail );

sub succeed { 1 }
sub fail    { 0 }

use Fatal qw( :void succeed fail );

1;
