#!perl
  
use strict;
use warnings;
  
use lib 'lib';
  
use Test::More tests => 3;
use Test::Output;
use Test::Output::Tie;
  
my $module = 'Logger';
use_ok( $module ) or exit;
  
stderr_is( sub { Logger::write_message( 'To STDERR!' ) }, 'To STDERR!',
    'write_message(  ) should write to STDERR by default' );
  
{
    local *Logger::OUTPUT_FH;
  
    my $out            = tie *Logger::OUTPUT_FH, 'Test::Output::Tie';
    $Logger::OUTPUT_FH = *Logger::OUTPUT_FH;
  
    Logger::write_message( 'To $out!' );
    is( $out->read(  ), 'To $out!', '... or to $OUTPUT_FH, if set' );
}
