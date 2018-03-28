#!perl

use strict;
use warnings;

use Test::More tests => 1;

my $module  = 'Chef';
use_ok( $module ) or exit;
