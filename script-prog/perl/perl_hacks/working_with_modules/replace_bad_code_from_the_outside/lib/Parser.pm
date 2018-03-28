package Parser;

sub parse
{
    my ($class, $text) = @_;
    validate_text( $shift );
    bless \$text, $class;
}

sub validate_text
{
    my $text = shift;
    exit 1 unless $text =~ /^</;
}

1;
