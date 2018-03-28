use Test::More;

eval 'use Test::Signature';

plan( skip_all => 'Test::Signature required for signature verification' )
    if $@;
plan( tests => 1 );
signature_ok(  );
