#!perl 
  
use strict;
use warnings;
  
use lib 'lib';
  
use Test::More tests => 6;
use Test::MockModule;
  
my $module = 'Logger';
use_ok( $module ) or exit;
  
can_ok( $module, 'log_message' );
  
{
    local $Logger::REPORT_TIME   = 0;
    local $Logger::REPORT_CALLER = 0;
  
    my $message;
    my $logger = Test::MockModule->new( 'Logger' );
    $logger->mock( write_message => sub { $message = shift } );
  
    Logger::log_message( 'no decoration' );
    is( $message, "no decoration\n",
        'log_message(  ) should not add time or caller unless requested' );
  
    $Logger::REPORT_TIME   = 1;
    Logger::log_message( 'time only' );
    (my $time = localtime(  )) =~ s/:\d+ /:\\d+ /;
    like( $message, qr/^\[$time\] time only$/,
        '... adding time if requested' );
  
    $Logger::REPORT_CALLER = 1;
    my $line               = __LINE__ + 1;
    Logger::log_message( 'time and caller' );
    like( $message, qr/^\[$time\] time and caller from main:$line in $0$/,
        '... adding time and caller, if both requested' );
  
    $Logger::REPORT_TIME   = 0;
    $line                  = __LINE__ + 1;
    Logger::log_message( 'caller only' );
    like( $message, qr/^caller only from main:$line in $0$/,
        '... adding caller only if requested' );
}
