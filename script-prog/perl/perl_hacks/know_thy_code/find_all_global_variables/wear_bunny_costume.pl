use vars qw( $frog $toad );

sub wear_bunny_costume
{
    my $bunny = shift;
    $frog     = $bunny;
    print "\$bunny is $bunny\n\$frog is $frog\n\$toad is $toad";
}
