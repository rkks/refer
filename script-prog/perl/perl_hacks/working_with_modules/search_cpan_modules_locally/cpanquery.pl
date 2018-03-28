#!perl -w

# import merrily
use strict;
use IO::Zlib;
use Parse::CPAN::Modlist;

# get the search pattern
my $pattern    = shift || die "You must pass a pattern\n";
my $pattern_re = qr/$pattern/;

# munge our name
my $self       = $0; $self =~ s!^.*[\\/]!!;

# naughty user
die ("usage : $self <query>\n") unless defined $pattern;

# get where the local modulelist is from CPAN(PLUS?)::Config
my $base;
eval { require CPANPLUS::Config; CPANPLUS::Config->import(); };
unless ($@)
{
    my $conf = CPANPLUS::Config->new();
    # different versions have the config in different places
    for (qw(conf _build))
    {
        $base = $conf->{$_}->{base}   if exists $conf->{$_};
    }
}

goto SKIP if defined $base;

eval { require CPAN::Config; CPAN::Config->import() };

unless ($@)
{
    local $CPAN::Config;
    $base = $CPAN::Config->{'keep_source_where'}."/modules/";
}

goto SKIP if defined $base;

die "Couldn't find where you keep your CPAN Modlist\n";

SKIP:
my $file     = "${base}/03modlist.data.gz";

# open the file and feed it to the mod list parser
my $fh       = IO::Zlib->new($file, "rb")  or die "Cannot open $file\n";
my $ml       = Parse::CPAN::Modlist->new(join "", <$fh>);

# by default we want colour
my $colour   = 1;

# check to see if we have Term::ANSIColor installed
eval { require Term::ANSIColor };

# but if we can't have it then we can't have it
$colour      = 0 if $@;

# now do the actual checking

my $first    = 0;

# check each module
for my $module (map { $ml->module($_) } $ml->modules())
{
    my $name = $module->name();
    my $desc = $module->description();

    # check to see if the pattern matches the name or desc   
    next unless  $name =~ /$pattern_re/i or $desc =~ /$pattern_re/i;

    # aesthetics
    print "\n-- Results for '$pattern' --\n\n" unless $first++;

    # check to see if it's installed
    eval  "require $name";   
       
    # print out the title - coloured if possible
    if ( $colour && !$@ )
    {
          print Term::ANSIColor::color('red'),
              "$name\n",
              Term::ANSIColor::color('reset');
    }
    elsif (!$@)
    {
        print "!! $name\n";
    }
    else
    {
        print "$name\n";
    }

    # print out the name and description           
    print "- $desc\n\n";
}

exit 0;
