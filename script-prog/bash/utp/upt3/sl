#!/usr/bin/perl

# Usage: sl [filenames]

die "Usage: sl [filenames]\n" unless @ARGV;

# Preliminaries.

$| = 1;
chop($cwd = `pwd`) || die "Can't find current directory: $!\n"
    if @ARGV > 1;
print "\n";

# Do each name.

foreach $name (@ARGV) {
    @indent = ();
    print "$name:\n";
    @path = split(m;/;, $name);

    # Make an absolute path relative to /.

    if (@path && $path[0] eq '') {
        chdir '/';
        shift @path;
        print '/';
        $indent = 1;
    }
    # Now follow the subdirectories and links.

    while (@path) {
        $elem = shift @path;
        $new = readlink($elem);
        if (defined $new) {     # A symbolic link.
            print "$elem \-> $new\n";
            $new =~ s!^\./!!;

            # Prepend symbolic link to rest of path.

            unshift(@path,split(m;/;, $new));

            # Deal with special cases.

            if (@path && $path[0] eq '') {

                # Absolute path starts over.

                chdir '/';
                shift @path;
                print '/';
                $indent = 1;
                @indents = ();
                next;
            }

            # Back up the tree as necessary.

            while (@indents && $path[0] eq '..') {
                $indent = pop(@indents);
                chdir '..'
                    || die "\n\nCan't cd to ..: $!\n";
                shift @path;
            }

            print "\t" x ($indent / 8), ' ' x ($indent % 8);
        }
        else {                  # An ordinary directory.
            print $elem;
            push(@indents,$indent);
            $indent += length($elem) + 1;
            if (@path) {
                print '/';
                chdir $elem
                    || die "\n\nCan't cd to $elem: $!\n";
            }
        }
    }
    print "\n\n";
    $indent = 0;
    chdir $cwd || die "Can't cd back: $!\n" if $cwd ne '';
}
