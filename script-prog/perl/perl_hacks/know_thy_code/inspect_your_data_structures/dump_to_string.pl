use IO::Scalar;

my $dump_str;
my $io  = IO::Scalar->new(\$dump_str);
my $oio = select($io);

print '<pre>',"\n";       # goes to $dump_str
$d->dumpvalue(\$someref); # as does this
print '</pre>';           # and this too

select($oio);             # old filehandle
print $dump_str;          # stdout again when you want it to
