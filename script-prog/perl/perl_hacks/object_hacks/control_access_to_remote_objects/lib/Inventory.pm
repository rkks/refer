package Inventory;

use Proxy::AccessControl;

sub insert :perms( 'create' )
{
    my ($self, $attributes) = @_;
    # ...
}

sub delete :perms( 'delete' )
{
    my ($self, $id) = @_;
    # ...
}

sub update :perms( 'write' )
{
    my ($self, $id, $attributes) = @_;
    # ...
}

sub fetch :perms( 'read' )
{
    my ($self, $id) = @_;
    # ...
}

1;
