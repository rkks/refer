#!/usr/bin/perl

use Lingua::EN::NameParse;

my $Name_Obj = Lingua::EN::NameParse->new(auto_clean  => 1);
my @names = <STDIN>;
for my $line (sort by_lastname @names){
  chomp($line);
  print $line, "\n";
}

sub by_lastname {
  my @names;
  for my $name ($a, $b) {
    chomp($name);
    if( my $err = $Name_Obj->parse($name) ){
      warn "WARN: Unparsable name ($name): $err";
    }
    my %tmp = $Name_Obj->components;
    push @names, \%tmp;
  }
  return lc $names[0]->{surname_1} cmp lc $names[1]->{surname_1};
}
