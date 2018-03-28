package Devel::Command::HelloWorld;
use base 'Devel::Command';

sub command
{
    print DB::OUT "Hello world!\n";
    1;
}

1;
