use Test::More;
use CGI;

if ($CGI->VERSION >= 3.11)
{
    plan skip_all => 'skipping compatibility tests for old CGI.pm';
}
else
{
    plan 'tests' => 17;
}
