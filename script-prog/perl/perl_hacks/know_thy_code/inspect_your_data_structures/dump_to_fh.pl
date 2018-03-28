open my $fh, '>dump.out';
my $old_fh = select($fh);
$d->dumpValue(\$ref);
close $fh;
select($old_fh);
