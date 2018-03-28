use aliased 'My::App::Contact';
use aliased 'My::App::Type::Contact' => 'ContactType';

my $contact_type = ContactType->new();
my $contact      = Contact->new({ type => $contact_type });
