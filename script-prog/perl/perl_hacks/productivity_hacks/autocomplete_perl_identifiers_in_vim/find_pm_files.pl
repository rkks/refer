use File::Find 'find';

# Where to create this list...
my $LIST_DIR  = "$ENV{HOME}/.vim_extras/"
my $LIST_FILE = "file_that_lists_every_installed_Perl_module";

# Make sure the directory is available...
unless (-e $LIST_DIR )
{
    mkdir $LIST_DIR
        or die "Couldn't create directory $LIST_DIR ($!)\n";
}

# (Re)create the file...
open my $fh, '>', "$LIST_DIR$LIST_FILE"
    or die "Couldn't create file '$LIST_FILE' ($!)\n";

# Only report each module once (the first time it's seen)...
my %already_seen;

# Walk through the module include directories, finding .pm files...
for my $incl_dir (@INC)
{
    find
    {
        wanted => sub
        {
            my $file = $_;

            # They have to end in .pm...
            return unless $file =~ /\.pm\z/;

            # Convert the path name to a module name...
            $file =~ s{^\Q$incl_dir/\E}{};
            $file =~ s{/}{::}g;
            $file =~ s{\.pm\z}{};

            # Handle standard subdirectories (like site_perl/ or 5.8.6/)...
            $file =~ s{^.*\b[a-z_0-9]+::}{};
            $file =~ s{^\d+\.\d+\.\d+::(?:[a-z_][a-z_0-9]*::)?}{};
            return if $file =~ m{^::};

            # Print the module's name (once)...
            print {$fh} $file, "\n" unless $already_seen{$file}++;
        },
        no_chdir => 1,
    }, $incl_dir;
}
