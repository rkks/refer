my @command = caller( 3 );

return if @command and $command[1] ne '-e';
