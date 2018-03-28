package MailTemplate;
  
use strict;
use Email::Send 'SMTP';
  
sub new
{
    my ($class, %args) = @_;
    bless \%args, $class;
}
  
BEGIN
{
    no strict 'refs';
  
    for my $accessor (qw( message recipients sender sender_address server ))
    {
        *{ $accessor } = sub
        {
            my $self   = shift;
            return $self->{$accessor};
        };
    }
}
  
sub add_recipient
{
    my ($self, $name, $address) = @_;
    my $recipients              = $self->recipients(  );
    $recipients->{$name}        = $address;
}
  
sub deliver
{
    my $self       = shift;
    my $recipients = $self->recipients(  );
  
    while (my ($name, $address) = each %$recipients)
    {
        my $message = $self->format_message( $name, $address );
        send( 'SMTP', $message, $self->server(  ) );
    }
}
  
sub format_message
{
    my ($self, $name, $address) = @_;
  
    my $message    = $self->message(  );
    my %data       =
    (
        name           => $name,
        address        => $address,
        sender         => $self->sender(  ),
        sender_address => $self->sender_address(  ),
    );
  
    $message =~ s/{(\w+)}/$data{$1}/g;
    return $message;
}
  
1;
