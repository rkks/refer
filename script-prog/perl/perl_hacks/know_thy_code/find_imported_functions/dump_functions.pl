use Devel::Symdump;
my $symbols   = Devel::Symdump->new( 'main' );
my @functions = $symbols->functions();
