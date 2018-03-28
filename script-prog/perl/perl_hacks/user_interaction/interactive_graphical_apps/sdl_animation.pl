use SDL;
use SDL::App;
use strict;

# specify the target animation speed here, in milliseconds between two
# frames; for 50 frames per second, this is 20 ms 
our $TARGET_ANIM_SPEED = 20;

# define an array where to store all the I<rectangles> changed between two
# frames; this allows faster screen updates than using SDL::App#flip
our @update_rects;

# initialize the background I<surface> to an image file if user specified one
# on the commandline, to blank otherwise

our $background = SDL::Surface->new(-f $ARGV[0] ? (-name   => $ARGV[0])
                                                : (-width  => 640,
                                                   -height => 480     ));
# open a 640x480 window for the application
our $app = SDL::App->new(-width => 640, -height => 480);

# copy the whole background I<surface> to the application window
$background->blit(undef, $app, undef);

# update the application window
$app->flip;

# define an array where to store all the I<surfaces> representing
# the colored sprites with all the levels of color and transparency
our @imgs = map
{
    # create a 30x20 I<surface> for one sprite
    my $surface = SDL::Surface->new(
        -width => 30, -height => 20, -depth => 32
    );

    # fill the I<surface> with a solid color; let it fade from
    # blue to white while the mapped int value is iterated over
    $surface->fill(undef,
        SDL::Color->new(-r => 128+$_*255/45, -g => 128+$_*255/45, -b => 255 )
    );

    # set the transparency of the I<surface> (more and more transparent)
    $surface->set_alpha(SDL_SRCALPHA, (15-$_)*255/15);

    # convert the I<surface> to the display format, to allow faster blits 
    # to the application window
    $surface->display_format();

} (1..15);

# define a helper function to blit a I<surface> at a given position on the
# application window, adding the I<rectangle> involved to the array of needed
# updates

sub blit_at
{
    my ($surface, $x, $y) = @_;
    my $dest_rect = SDL::Rect->new(
        -width => $surface->width(), -height => $surface->height(),
        -x => $x, '-y' => $y
    );
    $surface->blit(undef, $app, $dest_rect);
    push @update_rects, $dest_rect;
}

# define a helper function to blit the portion of background similar to the
# area of a I<surface> at a given position on the application window, adding
# the I<rectangle> involved to the array of needed updates; this actually
# "erases" the I<surface> previously blitted there 
sub erase_at
{
    my ($surface, $x, $y) = @_;
    my $dest_rect = SDL::Rect->new(
        -width => $surface->width(), -height => $surface->height(),
        -x => $x, '-y' => $y
    );
    $background->blit($dest_rect, $app, $dest_rect);
    push @update_rects, $dest_rect;
}

# define an array to store the positions of the sprites, a counter to
# calculate new positions of the sprite while it's animated, and a boolean to
# know if the animation has stopped or not
our (@pos, $counter, $stopped);

# define an instance of SDL::Event for event monitoring
our $event = SDL::Event->new();

# start the main loop here
while (1)
{
    # store the current value of the sdlperl milliseconds counter; the end of
    # the mainloop uses it for animation synchronization
    my $synchro_ticks = $app->ticks();

    # erase all sprites at their current positions (stored by @pos)
    for (my $i = 0; $i < @pos; $i++)
    {
        erase_at($imgs[$i], $pos[$i]{'x'}, $pos[$i]{'y'});
    }

    # ask for new events
    $event->pump();

    if ($event->poll != 0)
    {
        # if the event is a key press, stop the animation
        if ($event->type() == SDL_KEYDOWN)
        {
            $stopped = 1;
        }

        # if the event is a key release, resume the animation
        if ($event->type() == SDL_KEYUP)
        {
            $stopped = 0;
        }

        # if we receive a "QUIT" event (user clicked the "close" icon of the
        # application window) or the user hit the Escape key, exit program 
        if ($event->type == SDL_QUIT ||
            $event->type == SDL_KEYDOWN && $event->key_sym == SDLK_ESCAPE)
        {
            die "quit\n";
        }
    }

    # if the animation is not stopped, increase the counter
    $stopped or $counter++;

    # insert a new position in top of @pos; let positions be a sine-based
    # smooth curve
    unshift @pos,
    {
        'x' => 320 + 200 * sin($counter/30),
        'y' => 240 +  80 * cos($counter/25),
    };

    # remove the superfluous positions
    @pos > 15 and pop @pos;

    # draw all sprites at their new positions
    for (my $i = @pos - 1; $i >= 0; $i--)
    {
        blit_at($imgs[$i], $pos[$i]{'x'}, $pos[$i]{'y'});
    }

    # tell sdlperl to flush all updates in the specified I<rectangles>
    $app->update(@update_rects);

    # empty the array of I<rectangles> needing an update
    @update_rects = ();

    # wait the time necessary for this frame to last the target number of
    # milliseconds of a frame.  This allows the animation to look smooth
    my $to_wait = $TARGET_ANIM_SPEED - ($app->ticks - $synchro_ticks);
    $to_wait > 0 and $app->delay($to_wait);
}
