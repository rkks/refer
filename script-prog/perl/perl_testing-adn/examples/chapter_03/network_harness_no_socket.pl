use LWP::Simple;
use Test::Harness::Straps;
  
my $strap   = Test::Harness::Straps->new(  );
my $output  = get( 'http://testbox/tests/smoketest.t' );
my @lines   = split( /\n/, $output );
my %results = $strap->analyze( 'testbox smoketest', \@lines );
  
# parse %results as normal
