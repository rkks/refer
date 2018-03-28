use SDL::App;

# open a 640x480 window for your application
our $app = SDL::App->new(-width => 640, -height => 480);

# create a surface out of an image file specified on the command-line
our $img = SDL::Surface->new( -name => $ARGV[0] );

# blit the surface onto the window of your application
$img->blit( undef, $app, undef );

# flush all pending screen updates
$app->flip();

# sleep for 3 seconds to let the user view the image
sleep 3;
