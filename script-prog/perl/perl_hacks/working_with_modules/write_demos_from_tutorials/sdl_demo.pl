use SDL::App;

# change these values as necessary
my  $title                   = 'My SDL App';
my ($width, $height, $depth) = ( 640, 480, 16 );

my $app = SDL::App->new(
    -width  => $width,
    -height => $height,
    -depth  => $depth,
    -title  => $title,
);

# your code here; remove the next line
sleep 2;
