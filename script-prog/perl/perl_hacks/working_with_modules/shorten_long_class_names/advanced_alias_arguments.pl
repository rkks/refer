use aliased 'My::App::Contact' => 'Contact', qw( EMAIL PHONE );

my $contact = Contact->new({
    kind  => EMAIL,
    value => 'perlhacks@oreilly.com',
});
