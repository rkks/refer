#!/usr/bin/perl

use strict;
use File::Find;

if (@ARGV < 2)
{
    die "Usage: $0 [prereq_scenarios_dir] [args to prove]\n";
}

my $scenarios_dir = shift;

my %scenario_modules;
my $errors;

my @scenarios     = grep { -d } <$scenarios_dir/*>;

for my $lib_dir (@scenarios)
{
    unless (-d $lib_dir)
    {
        $errors   = 1;
        warn "lib dir does not exist: $lib_dir\n";
        next;
    }
    my @modules;

    find(sub
    {
        return unless -f;

        my $dir =  "$File::Find::dir/$_";
        $dir    =~ s/^\Q$lib_dir\E//;
        $dir    =~ s/\.pm$//;
        $dir    =~ s{^/}{};
        $dir    =~ s{/}{::}g;

        push @modules, $dir;
    }, $lib_dir);

    $scenario_modules{$lib_dir} = \@modules;
}

die "Terminating." if $errors;

for my $lib_dir (@scenarios)
{
    my $modules   = join ', ', sort @{ $scenario_modules{$lib_dir} };
    $modules    ||= 'none';
    print "\n" . '#' x 62 . "\n";
    print "Running tests.  Old (or absent) modules in this scenario:\n";
    print "$modules\n";

    my @prove_command = ('prove', "-I$lib_dir", @ARGV);

    system( @prove_command ) && do
    {
        die <<EOF;
##############################################################
One or more tests failed in scenario $lib_dir.
The old or absent modules were:
    $modules

The command was:
    @prove_command

Terminating.
##############################################################
EOF
    };
}
