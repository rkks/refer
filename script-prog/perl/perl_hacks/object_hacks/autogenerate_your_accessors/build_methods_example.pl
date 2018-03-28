use Class::BuildMethods
  'name',
  gender => { default  => 'male' },
  age    => { validate => sub
  {
      my ($self, $age) = @_;
      carp 'You can't enlist if you're a minor'
          if ( $age < 18 && ! $self->is_emancipated() );
  }};
