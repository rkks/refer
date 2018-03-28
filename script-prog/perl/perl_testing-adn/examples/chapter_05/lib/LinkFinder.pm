package LinkFinder;
  
use URI;
use LWP::Simple (  );
use HTML::TokeParser::Simple;
  
sub new
{
    my ($class, $html) = @_;
    my $uri            = URI->new( $html );
  
    if ($uri->scheme(  ))
    {
        $html = LWP::Simple::get( $uri->as_string(  ) );
    }
  
    my $self = bless { html => $html }, $class;
    $self->reset(  );
}
  
sub parser
{
    my $self = shift;
    return $self->{parser};
}
  
sub html
{
    my $self = shift;
    return $self->{html};
}
  
sub find_links
{
    my ($self, $uri) = @_;
    my $parser       = $self->parser(  );
  
    my @links;
  
    while (my $token = $parser->get_token(  ) )
    {
        next unless $token->is_start_tag( 'a' );
        next unless $token->get_attr( 'href' ) =~ /\Q$uri\E/;
        push @links, $self->find_text(  );
    }
  
    return @links;
}
  
sub find_text
{
    my $self   = shift;
    my $parser = $self->parser(  );
  
    while (my $token = $parser->get_token(  ))
    {
        next unless $token->is_text(  );
        return $token->as_is(  );
    }
  
    return;
}
  
sub reset
{
    my $self        = shift;
    my $html        = $self->html(  );
    $self->{parser} = HTML::TokeParser::Simple->new( string => $html );
  
    return $self;
}
  
1;
