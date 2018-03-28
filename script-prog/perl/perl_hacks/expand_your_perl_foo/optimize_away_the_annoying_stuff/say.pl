sub say { print @_, "\n" }

# and ever after...

say "Showing first ", @results / 2, " of ", scalar @results;

for (@results[ 0 .. @results / 2 - 1 ])
{
    if (m/($IDENT): \s* (.*?) \s* $/x)
    {
        say "$1 -> ", normalize($2);
    }
    else
    {
        say "Strange result: ", substr( $2, 0, 10 );
    }
}
