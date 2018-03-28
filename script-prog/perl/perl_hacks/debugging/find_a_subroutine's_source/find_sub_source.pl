  package My::Package;

  use Sub::Identify ':all';
  use HTML::Entities 'encode_entities';
  print stash_name( \&encode_entities );