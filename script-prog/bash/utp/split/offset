#!/usr/local/bin/perl
# offset - indent each line (default: 5 spaces); write to stdout
# Usage: offset [-##] [files]

if ($ARGV[0] =~ /-[0-9]+/) {
    ($indent = $ARGV[0]) =~ s/-//;
    shift @ARGV;
} else {
    $indent = 5;
}

while (<>) {
    print " " x $indent, $_;
}
