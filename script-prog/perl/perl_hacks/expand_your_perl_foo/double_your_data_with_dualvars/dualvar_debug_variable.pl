use Scalar::Util 'dualvar';

my $width   = dualvar( 800, 'Screen width'       );
my $height  = dualvar( 600, 'Screen height'      );
my $colors  = dualvar(  16, 'Screen color depth' );

# some code
sub debug_variable
{
    my $constant = shift;
    printf STDERR "%s is %d\n", $constant, $constant;
}
