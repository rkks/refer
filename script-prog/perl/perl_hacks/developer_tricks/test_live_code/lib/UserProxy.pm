package UserProxy;

use strict;
use warnings;

use User;
use Test::Builder;

sub new
{
    my ($class, %args) = @_;
    my $proxied        = User->new( %args );
    my $Test           = Test::Builder->create();
    $Test->output( time() . '_' . $proxied->id() . '.tap' );
    $Test->plan( 'no_plan' );
    bless { proxied => $proxied, test => $Test }, $class;
}

sub proxied
{
    my $self = shift;
    return $self->{proxied};
}

sub test
{
    my $self = shift;
    return $self->{test};
}

sub can
{
    my ($self, $method) = @_;
    my $proxied         = $self->proxied();
    return $proxied->can( $method );
}

sub verify_name
{
    my ($self, $name)   = @_;
    my $proxied         = $self->proxied():
    my $test            = $self->test();
    $test->ok( $proxied->verify_name( $name ), "verify_name() for '$name'" )
        || $test->diag( $proxied->verification_error() );
}

# ...

1;
