package Test::Between;
  
use strict;
use warnings;
  
use base 'Exporter';
  
our@EXPORT = qw( is_between );
  
use Test::Builder;
my $Test = Test::Builder->new(  );
  
sub is_between ($$$;$)
{
    my ($item, $lower, $upper, $desc) = @_;
  
    return
    (
        $Test->ok( "$lower" le "$item" && "$item" le "$upper", $desc ) ||
        $Test->diag( "        $item is not between $lower and $upper" )
    );
}
  
1;
