use Test::More;
use Test::PerPerlHelper;
if (eval { require PersistentPerl } and PersistentPerl->i_am_perperl() )
{
    Test::PerPerlHelper->plan(
        'skip_all',
        'Redirecting STDIN doesn't work under perperl' );
}
else
{
    plan "no_plan";
}
