#!/usr/bin/perl

use strict;
use warnings;

use Perl::Tidy;
use File::Temp;
use File::Spec::Functions;

my $svnlook      = '/usr/bin/svnlook';
my $diff         = '/usr/bin/diff -u';

# eat the arguments so as not to confuse Perl::Tidy
my ($repo, $rev) = @ARGV;
@ARGV            = ();

my @diffs;

for my $changed_file (get_changed_perl_files( $repo, $rev ))
{
    my $source = get_revision( $repo, $rev, $changed_file );
    Perl::Tidy::perltidy( source => \$source, destination => \(my $dest) );
    push @diffs, get_diff( $changed_file, $source, $dest );
    report_diffs( @diffs );
}

sub get_changed_perl_files
{
    my ($repo, $rev) = @_;

    my @files;

    for my $change (`$svnlook changed $repo -r $rev`)
    {
        my ($status, $file) =  split( /\s+/, $change );
        next unless $file   =~ /\.p[lm]\z/;
        push @files, $file;
    }

    return @files;
}

sub get_revision
{
    my ($repo, $rev, $file) = @_;
    return scalar `$svnlook cat $repo -r $rev $file`;
}

sub get_diff
{
    my $filename        = shift;
    return if $_[0] eq $_[1];

    my $dir   = File::Temp::tempdir();
    my @files = map { catdir( $dir, $filename . $_ ) } qw( .orig .tidy );

    for my $file (@files)
    {
        open( my $out, '>', $file ) or die "Couldn't write $file: $!\n";
        print $out shift;
        close $out;
    }

    return scalar `$diff @files`;
}

sub report_diffs
{
    for my $diff (@_)
    {
        warn "Error:\n$diff\n";
    }
}
