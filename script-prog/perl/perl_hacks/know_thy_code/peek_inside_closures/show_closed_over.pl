use Data::Dumper;
use PadWalker 'closed_over';

my $hundred_by_nines = make_counter( 0, 100, 9 );

while ( my $item = $hundred_by_nines->() )
{
    my $vars = closed_over( $hundred_by_nines );
    warn Dumper( $vars );
}
