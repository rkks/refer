use Devel::Symdump;

my %existing;

BEGIN
{
    my $symbols = Devel::Symdump->new( 'main' );
    @existing{ $symbols->functions() } = ();
}

use File::Spec::Functions;

BEGIN
{
    my $symbols   = Devel::Symdump->new( 'main' );
    my @new_funcs =
        map { s/main:://; $_ }
        grep { not exists $existing{ $_ } } $symbols->functions();
    local $" = "\n  ";
    warn qq|Imported:$"@new_funcs\n|;
}
