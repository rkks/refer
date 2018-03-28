#!perl

use strict;
use warnings;

use Test::More tests => 5;

my $module  = 'Chef';
use_ok( $module ) or exit;

for my $export (qw( slice dice fricassee boil ))
{
    can_ok( __PACKAGE__, $export );
}
