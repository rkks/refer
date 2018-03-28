# use model and view classes

# create the appropriate model objects
my $uncle  = Uncle->new(
    name => 'Bob', occupation => 'Uncle', age => 50
);
my $nephew = Nephew->new(
    name => 'Jacob', occupation => 'Agent of Chaos', age => 3
);

# apply the appropriate views
Class::Trait->apply( $uncle,  'TextView' );
Class::Trait->apply( $nephew, 'YAMLView' );

# display the results
$uncle->render();
$nephew->render();
