use Test::More tests => 1;
use Test::Differences;
  
my $string1 = <<"END1";
Lorem ipsum dolor sit
amet, consectetuer
adipiscing elit.
END1
  
my $string2 = <<"END2";
Lorem ipsum dolor sit
amet, facilisi
adipiscing elit.
END2
  
eq_or_diff( $string1, $string2, 'are they the same?' );
