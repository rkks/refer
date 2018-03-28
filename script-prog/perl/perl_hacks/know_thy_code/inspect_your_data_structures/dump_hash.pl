use strict;
use Dumpvalue;

my $d    = Dumpvalue->new();
my $hash =
{
     first_name => 'Tim',
     last_name  => 'Allwine',
     friends    => [ 'Jon','Nat','Joe' ],
};
$d->dumpValue(\$hash);
