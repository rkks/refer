#!perl

use strict;
use warnings;

use IPC::Run 'run';
use Test::More tests => 7;

my ($out, $err) = runperl( '-v' );
like($out, qr/This is perl/, '-v should print short version message'      );
is(  $err, '',               '... and no error'                           );

($out, $err)    = runperl( '-V' ) ;
like($out, qr/Compiled at/,  '-V should print extended version message'   );
is(  $err, '',               '... and no error'                           );

($out, $err)    = runperl(qw( -e x++ ));
like($err, qr/Can't modify constant.+postincrement/,
                            'constant modification should die with error' );
like( $err, qr/Execution.+aborted.+compilation errors/,
                        '... aborting with to compilation errors'     );
is( $out, '',           '... writing nothing to standard output'      );

sub runperl
{
    run( [ $^X, @_ ], \my( $in, $out, $err ) );
    return ($out, $err);
}
