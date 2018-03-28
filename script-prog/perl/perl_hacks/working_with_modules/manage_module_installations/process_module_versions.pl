use Module::CoreList;

my ($bundle, $version) = @ARGV;
$version             ||= $];
@ARGV                  = $bundle;
my $core_list          = $Module::CoreList::version{ $version };
die "Unknown version $version\n" unless $core_list;

# find module list
while (<>)
{
    print;
    last if $_ eq "=head1 CONTENTS\n";
}

print "\n";

# process only module/version lines
while (<>)
{
    if ( $_ eq "=head1 CONFIGURATION\n" )
    {
        print;
        last;
    }

    chomp;
    next unless $_;

    my ($module, $version) = split( /\s+/, $_ );
    $version = 0 if $version eq 'undef';

    next if exists $core_list->{ $module }
               and $core_list->{ $module } >= $version;

    print "$module $version\n\n";
}

# print everything else
print while <>;
