use Test::More;
  
if ( [ localtime ]->[6] != 2 )
{
    plan( skip_all => 'only run these tests on Tuesday' );
}
else
{
    plan( tests => 1 );
}
  
require Tuesday;
my $day = Tuesday->new(  );
ok( $day->coat(  ), 'we brought our coat' );
