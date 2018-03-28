use DispatchBot;

my $bot = DispatchBot->new(
    server   => "irc.perl.org",
    port     => "6667",
    channels => ["#bottest"],
    nick     => 'rebot',
);
$bot->run();
