package Test::PerPerlHelper;

use strict;
use warnings;

use base 'Test::Builder';

require Test::More;

sub import
{
    my $class = shift;

    if (eval {require PersistentPerl} && PersistentPerl->i_am_perperl())
    {
        # rebless the Test::Builder singleton into our class
        # so that we can override the plan and _dup_stdhandles methods
        my $Test = Test::Builder->new();
        bless $Test, __PACKAGE__;
    }

    $class->plan(@_);
}

sub plan
{
    my $class = shift;
    return unless @_;

    my $Test  = Test::Builder->new();

    if (eval {require PersistentPerl} && PersistentPerl->i_am_perperl())
    {
        $Test->reset();

        Test::Builder::_autoflush(\*STDOUT);
        Test::Builder::_autoflush(\*STDERR);

        $Test->output(\*STDOUT);
        $Test->failure_output(\*STDERR);
        $Test->todo_output(\*STDOUT);

        $Test->no_ending(1);
        my $pp   = PersistentPerl->new();
        $pp->register_cleanup(sub { $Test->_ending });
    }
    $Test->SUPER::plan(@_);
}

# Duplicating STDERR and STDOUT doesn't work under perperl
# so override it with a no-op
sub _dup_stdhandles {  }

1;
