eval 'require URI;';
if ($@)
{
    plan skip_all => 'optional module URI not installed';
}
else
{
    plan 'tests' => 10;
}
