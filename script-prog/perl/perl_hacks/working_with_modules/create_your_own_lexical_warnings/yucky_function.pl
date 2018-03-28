sub yucky_function
{
    my ($package, $filename, $line) = caller();

    warnings::warnif( 'deprecated',
        "yucky_function() is deprecated at $filename:$line\n" );

    goto &yummy_function;
}
