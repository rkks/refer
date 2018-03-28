package Sub::With::Params;
use Filter::Simple;

# Regex that matches a valid Perl identifier (e.g. a sub name)...
my $IDENT = qr/[^\W\d]\w*/;

# Apply this filter to the code of any program
# that uses Sub::With::Params...
FILTER_ONLY code => sub
{
    s{ ( sub \s* $IDENT \s* )   # Match any named sub declaration
       (   \( .*? \)        )   # ...followed by a parameter list
       (   \s* \{           )   # ...followed by a sub body
    }
    {$1$3 my $2 = \@_;}gxs;     # Then move the param list inside the
                                # sub, converting it to a list of
                                # lexical variables initialized from @_
};

1;
