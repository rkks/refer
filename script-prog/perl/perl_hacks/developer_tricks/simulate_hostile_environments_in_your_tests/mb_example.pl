my $builder = Module::Build->new(
  # ... other Build.PL options ...
  requires =>
  {
      'Test::More'       => 0,
      'CGI'              => 2.0,
  }
);
