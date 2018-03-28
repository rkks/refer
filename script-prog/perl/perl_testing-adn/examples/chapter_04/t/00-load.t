use Test::More tests => 2;

BEGIN
{
    use_ok( 'Taxes::Autocomplete' );
    use_ok( 'Taxes::Loophole' );
}

diag( "Testing Taxes::Autocomplete $Taxes::Autocomplete::VERSION,
       Perl 5.008004, /usr/bin/perl" );
