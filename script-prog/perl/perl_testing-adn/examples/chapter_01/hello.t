#!perl
  
use strict;
use warnings;
  
use Test::Simple tests => 1;
  
sub hello_world
{
    return "Hello, world!";
}
  
ok( hello_world(  ) eq "Hello, world!" );
