use Net::SSH::Perl;
use Test::Harness::Straps;
  
my $strap   = Test::Harness::Straps->new(  );
my $ssh     = Net::SSH::Perl->new( 'testbox' );
$ssh->login(qw( username password ));
  
my ($stdout, $stderr, $exit) = $ssh->cmd( 'runtests' );
my %results = $strap->analyze_fh( 'testbox tests', $stdout );
  
# parse %results as normal
