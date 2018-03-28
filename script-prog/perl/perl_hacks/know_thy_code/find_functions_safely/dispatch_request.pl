sub dispatch_request
{
    my ($self, $q) = @_;
    my $action     = $q->param( 'action' );
    $self->$action();
}
