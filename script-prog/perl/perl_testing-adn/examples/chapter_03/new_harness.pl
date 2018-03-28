#!perl
  
use strict;
use warnings;
  
use Test::Harness::Straps;
my $strap = Test::Harness::Straps->new();
  
for my $file (@ARGV)
{
    next unless -f $file;
    my %results = $strap->analyze_file( $file );
    printf <<END_REPORT, $file, @results{qw( max seen ok skip todo bonus )};
Results for %s
    Expected tests:    %d
    Tests run:         %d
    Tested passed:     %d
    Tests skipped:     %d
    TODO tests:        %d
    TODO tests passed: %d
END_REPORT
}
