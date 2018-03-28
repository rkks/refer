sub dispatch_request
{
    my ($self, $q) = @_;
    my $action     = 'action_' . $q->param( 'action' );
    return unless $self->can( $action );
    $self->$action();
}
