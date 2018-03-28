package B::Lint::VoidSyscalls;

use strict;
use warnings;

use B::Lint;

# Make B::Lint accept plugins if it doesn't already.
use if ! B::Lint->can('register_plugin'),
    B::Lint::Pluggable;

# Register this plugin.
B::Lint->register_plugin( __PACKAGE__, [ 'void_syscall' ] );

# Check these opcodes
my $SYSCALL = qr/ ^ (?: open | print | close ) $ /msx;

# Also look for things that are right at the end of a subroutine
# sub foo { return print() }
my $TERM = qr/ ^ (?: leavesub ) $/msx;

sub match
{
    my ( $op, $checks ) = @_;

    if (     $checks->{void_syscall}
         and $op->name() =~ m/$SYSCALL/msx )
    {
        if ( $op->FLAGS & OPf_WANT_VOID )
        {
            warn "Unchecked " .  $op->name() .  " system call "
                .  "at " .  B::Lint->file() .  " on line " .
                .  B::Lint->line() .  "\n";
        }
        elsif ( $op->next->name() =~ m/$TERM/msx )
        {
            warn "Potentially unchecked " .  $op->name() .  " system call "
                .  "at " .  B::Lint->file() .  " on line "
                .  B::Lint->line() .  "\n";
        }
    }
}

1;
