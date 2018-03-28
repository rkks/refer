package Devel::Command::X;

use base 'Devel::Command';

sub command
{
    my ($cmd) = @_;

    if ($cmd  =~ /x marks/)
    {
        print DB::OUT "Arrrrr....\n";
        return 1;
    }
    else
    {
        return 0;
    }
}

1;
