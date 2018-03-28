package Heredoc::Indenting;

use Filter::Simple;

FILTER
{
    # Find all instances of...
    1 while
        s{ <<                     #     Heredoc marker
           ( ['"]             )   # $1: Quote for terminator
           ( (?:\\\1|[^\n])*? )   # $2: Terminator specification
             \1                   #     Matching closing quote
           ( [^\n]*  \n       )   # $3: The rest of the statement line
           ( .*? \n           )   # $4: The heredoc contents
           ( [^\S\n]*         )   # $5: Any whitespace indent before...
             \2 \n                #     ...the terminator itself
        }

        # ... and replace it with the same heredoc, with its terminator
        # outdented and the heredoc contents passed through a subroutine
        # that removes the indent from each line...
        {Try::outdent(q{$1$2$1}, '$5',<< $1$2$1)\n$4$2\n$3}xms;
};

use Carp;

# Remove indentations from a string...
sub outdent
{
    my ($name, $indentation, $string) = @_;

    # Complain if any line doesn't have the specified indentation...
    if ($string =~ m/^((?:.*\n)*?)(?!$indentation)(.*\S.*)\n/m)
    {
        my ($good_lines, $bad_line) = ($1, $2);
        my $bad_line_pos = 1 + ($good_lines =~ tr/\n/\n/);
        croak "Negative indentation on line $bad_line_pos ",
              "of <<$name heredoc specified";
    }

    # Otherwise remove the indentations from each line...
    $string =~ s/^$indentation//gm;
    return $string;
}

1;
