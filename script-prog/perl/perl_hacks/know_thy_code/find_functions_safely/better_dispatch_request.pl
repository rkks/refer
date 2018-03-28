sub dispatch_request
{
    my ($self, $q) = @_;
    my $action     = $q->param( 'action' );
    return unless $self->can( $action );
    $self->$action();
}
