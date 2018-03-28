#!perl
  
use strict;
use warnings;
  
use Test::More tests => 23;
use Test::MockObject;
  
use lib 'lib';
  
$INC{'Net/SMTP.pm'} = 1;
my $module          = 'MailTemplate';
my $message         = do { local $/; <DATA> };
  
use_ok( $module ) or exit;
  
can_ok( $module, 'new' );
my $mt = $module->new(
    server         => 'smtp.example.com',
    sender         => 'A. U. Thor',
    message        => $message,
    sender_address => 'author@example.com',
    recipients     => { Bob => 'bob@example.com' },
);
isa_ok( $mt, $module );
  
can_ok( $mt, 'server' );
is( $mt->server(  ), 'smtp.example.com',
    'server(  ) should return server set in constructor' );
  
can_ok( $mt, 'add_recipient' );
$mt->add_recipient( Alice => 'alice@example.com' );
  
can_ok( $mt, 'recipients' );
is_deeply( $mt->recipients(  ),
           { Alice => 'alice@example.com', Bob => 'bob@example.com' },
           'recipients(  ) should return all recipients' );
  
can_ok( $mt, 'deliver' );
  
my $smtp = Test::MockObject->new(  );
$smtp->fake_module( 'Net::SMTP', new => sub { $smtp } );
$smtp->set_true( qw( mail to data -quit ) );
$mt->deliver(  );
  
my %recipients =
(
    Alice => 'alice@example.com',
    Bob   => 'bob@example.com', 
);
  
while (my ($name, $address) = each %recipients)
{
    my ($method, $args) = $smtp->next_call(  );
    is( $method,    'mail',              'deliver(  ) should open a mail' );
    is( $args->[1], 'author@example.com','... setting the From address' );
  
    ($method, $args) = $smtp->next_call(  );
    is( $method,    'to',                    '... then the To address'  );
    is( $args->[1], $address,                '... for the recipient'    );
  
    ($method, $args) = $smtp->next_call(  );
    is( $method,      'data',             '... sending the message'     );
    like( $args->[1], qr/Hello, $name/,   '... greeting the recipient'  );
    like( $args->[1], qr/Love,.A. U. Thor/s,
          '... and signing sender name' );
}
  
__DATA__
To: {address}
From: {sender_address}
Subject: A Test Message
  
Hello, {name}!
  
You won't actually receive this message!
  
Love,
{sender}
