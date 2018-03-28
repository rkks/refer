open my $fh, '>:via(Scream):via(Cookiemonster)',
   'author_bio.txt' or die $!;

print $fh "I eat cookies without cease or restraint.\n",
   "I like cookies.\n";

close($fh);
