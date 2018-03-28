package Scorekeeper;

use strict;
use warnings;

use Class::DBI::Loader;
use Class::DBI::Loader::Relationship;

my $loader = Class::DBI::Loader->new(
    dsn       => 'dbi:SQLite2:dbname=keeper.db',
    namespace => 'Scorekeeper',
);

$loader->relationship( 'a game has scores'              );
$loader->relationship( 'a player has games with scores' );

package Scorekeeper::Game;

sub is_winner
{
    my ( $self, $player ) = @_;

    my @scores =
        sort {
            return 0 unless $a and $b;
            $b->value() <=> $a->value()
        }
        $self->scores();
    return $player eq $scores[0]->player();
}

sub has_player
{
    my ( $self, $player ) = @_;

    ( $player == $_->player() ) && return 1 for $self->scores();
    return 0;
}

package Scorekeeper::Player;

sub wins
{
    my ($self) = @_;
    return scalar grep { $_->is_winner($self) } $self->games();
}

sub losses
{
    my ($self) = @_;
    return scalar( $self->games() ) - $self->wins();
}

sub winning_percentage_against
{
    my ( $self, $other ) = @_;

    my @all = grep { $_->has_player($other) } $self->games();
    my @won = grep { $_->is_winner($self) } @all;

    return @won / @all * 100;
}

sub retrieve_all_ranked
{
    my ($self) = @_;
    return sort { $b->wins() <=> $a->wins() }
        $self->retrieve_all();
}

sub opponents
{
    my ($self) = @_;

    my %seen;
    $seen{$_}++ for map { $_->player() } map { $_->scores() }
        $self->games();
    delete $seen{$self};

    return grep { exists $seen{$_} } $self->retrieve_all();
}

1;
