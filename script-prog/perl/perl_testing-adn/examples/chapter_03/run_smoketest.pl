#!perl

use strict;
use warnings;

use constant SENDER    => 'testers@example.com';
use constant RECIPIENT => 'smoketester@example.com';
use constant MAILHOST  => 'smtp.example.com';

use Cwd;
use SVN::Client;
use Email::Send;
use Test::Harness::Straps;

my $path     = shift || die "Usage:\n$0 <repository_path>\n";
my $revision = update_repos( $path );
my $failures = run_tests(    $path );

send_report( $path, $revision, $failures );

sub update_repos
{
    my $path     = shift;
    my $ctx      = SVN::Client->new(  );
    return $ctx->update( $path, 'HEAD', 1 );
}

sub run_tests
{
    my $path  = shift;
    my $strap = Test::Harness::Straps->new(  );
    my $cwd   = cwd(  );

    chdir( $path );

    my @failures;

    for my $test (<t/*.t>)
    {
        my %results = $strap->analyze_file( $test );
        next if $results{passing};

        push @failures,
        {
            file => $test,
            ok   => $results{ok},
            max  => $results{max},
        };
    }

    chdir( $cwd );

    return \@failures;
}

sub send_report
{
    my ($revision, $path, $failures) = @_;
    return unless @$failures;
 
    my $message = sprintf(<<END_HEADER, RECIPIENT, SENDER, $path, $revision);
To: %s
From: %s
Subect: Failed Smoketest For %s at Revision %d

END_HEADER

    for my $failure (@$failures)
    {
        $message .= sprintf( "%s:\n\tExpected: %d\n\tPassed: %d\n",
            @$failure{qw( file max ok )} );
    }

    send( 'SMTP', $message, MAILHOST );
}
