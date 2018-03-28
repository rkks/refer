package DispatchBot;

use strict;
use Regexp::Assemble;
use Bot::BasicBot;
use YAML qw(LoadFile DumpFile);

use vars qw( $VERSION @ISA );
$VERSION    = '0.03';
@ISA        = 'Bot::BasicBot';

my $factoid = _load( 'factoid.dat' ); # "foo" is "bar" factoids
my $karma   = _load( 'karma.dat' );   # keep track of foo++ and foo--

sub _load
{
    my $file = shift;
    return -e $file ? LoadFile($file) : {};
}

sub _save
{
    my ($dictionary, $file) = @_;
    DumpFile( $file, $dictionary );
}

sub _flush
{
    _save( $factoid, 'factoid.dat' );
    _save( $karma,   'karma.dat' );
}

END { _flush }

my %dispatch =
(
    # define a factoid
    '(\\S+) is (.*)$' => sub { $factoid->{$_[0]} = $_[1]; _flush; return },

    # query a factoid
    '(\\S+)\s*\\?$' => sub
    {
        exists $factoid->{$_[0]}
            and return "I believe that $_[0] is $factoid->{$_[0]}"
    },

    # drop a factoid
    'forget (\\S+)$'=> sub
    {
        if (exists $factoid->{$_[0]})
        {
            my $message = "I forgot $_[0]";
            delete $factoid->{$_[0]};
            _flush;
            return $message;
        }
    },

    # karma shifts
    '(\\S+)\\+\\+' => sub { $karma->{$_[0]}++; _flush; return },
    '(\\S+)--'     => sub { $karma->{$_[0]}--; _flush; return },

    # karma query
    '^karma (\\S+)$' => sub
    {
        return exists $karma->{$_[0]}
            ? "$_[0] has karma of $karma->{$_[0]}"
            : "$_[0] has neutral karma"
    },

    # time... to die
    '^!quit$' => sub { exit },
);

my $re = Regexp::Assemble->new->track->add(keys %dispatch);

sub said
{
    my ($self, $arg) = @_;
    $re->match($arg->{body})
        and return $dispatch{$re->matched}->($re->capture);
    return;
}

1;
