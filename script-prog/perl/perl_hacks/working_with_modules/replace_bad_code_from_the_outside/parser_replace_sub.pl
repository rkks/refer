use Parser; 
local *Parser::validate_text;
*Parser::validate_text = sub
{
    my $text = shift;
    die "Invalid text '$text'\n" unless $text =~ /^</;
};
