use Test::More tests => 18;
use Test::Exception;
use Test::Deep;

use strict;
use warnings;

BEGIN
{
    use_ok('Scorekeeper');
}

my $a = Scorekeeper::Player->create( { name => 'PlayerA' } );
my $b = Scorekeeper::Player->create( { name => 'PlayerB' } );
my $c = Scorekeeper::Player->create( { name => 'PlayerC' } );

END
{
    foreach my $player ( $a, $b, $c )
    {
        $player->games->delete_all();
        $player->delete();
    }
}

dies_ok { Scorekeeper::Player->create( { name => $a->name() } ) }
    'cannot create two players with the same name';

foreach my $tuple ( [ 11, 8 ], [ 9, 11 ], [ 11, 7 ], [ 10, 11 ], [ 11, 9 ] )
{
    my ( $score1, $score2 ) = @$tuple;

    my $g = Scorekeeper::Game->create( {  } );
    $g->add_to_scores( { player => $a, value => $score1 } );
    $g->add_to_scores( { player => $b, value => $score2 } );
}

my $g2 = Scorekeeper::Game->create( {  } );
$g2->add_to_scores( { player => $a, value => 11 } );
$g2->add_to_scores( { player => $c, value => 8 } );

is( scalar( $a->games() ), 6 );
is( scalar( $b->games() ), 5 );

is( $a->wins(),   4, "player A's wins"   );
is( $b->wins(),   2, "player B's wins"   );
is( $c->wins(),   0, "player C's wins"   );

is( $a->losses(), 2, "player A's losses" );
is( $b->losses(), 3, "player B's losses" );
is( $c->losses(), 1, "player C's losses" );

cmp_deeply( [ $a->opponents() ], bag( $b, $c ), "player A's opponents" );
is_deeply(  [ $b->opponents() ], [$a],          "player B's opponents" );
is_deeply(  [ $c->opponents() ], [$a],          "player C's opponents" );

is( $a->winning_percentage_against($b), 60,  'A vs B' );
is( $b->winning_percentage_against($a), 40,  'B vs A' );

is( $a->winning_percentage_against($c), 100, 'A vs C' );
is( $c->winning_percentage_against($a), 0,   'C vs A' );

is_deeply(
    [ Scorekeeper::Player->retrieve_all_ranked() ],
    [ $a, $b, $c ],
    'players retrieved in the correct order of rank'
);
