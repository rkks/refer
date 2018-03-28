#!perl
  
use strict;
use warnings;
  
use lib 'lib';
  
use Test::More tests => 11;
use Test::MockModule;
  
my $module = 'LinkFinder';
use_ok( $module ) or exit;
my $html   = do { local $/; <DATA> };
  
my $vanity = $module->new( $html );
isa_ok( $vanity, $module );
is( $vanity->html(  ), $html, 'new(  ) should allow HTML passed in from string' );
  
{
    my $uri;
    my $lwp = Test::MockModule->new( 'LWP::Simple' );
    $lwp->mock( get => sub ($) { $uri = shift; $html } );
  
    $vanity = $module->new( 'http://www.example.com/somepage.html' );
    is( $vanity->html(  ), $html, '... or from URI if passed' );
    is( $uri, 'http://www.example.com/somepage.html',
        '... URI passed into constructor' );
}
  
my @results = $vanity->find_links( 'http' );
is( @results, 3, 'find_links(  ) should find all matching links' );
is( $results[0], 'one author',     '... in order'              );
is( $results[1], 'another author', '... of appearance'         );
is( $results[2], 'a project',      '... in document'           );
  
$vanity->reset(  );
@results    = $vanity->find_links( 'perl' );
is( @results, 1,              'reset(  ) should reset parser'    );
is( $results[0], 'a project', '... allowing more link finding' );
  
__DATA__
<html>
<head><title>some page</title>
<body>
<p><a href="http://wgz.org/chromatic/">one author</a></p>
<p><a href="http://langworth.com/">another author</a></p>
<p><a href="http://qa.perl.org/">a project</a></p>
</body>
