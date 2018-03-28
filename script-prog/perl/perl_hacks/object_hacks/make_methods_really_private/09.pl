  package Class::HideMethods;

  use strict;
  use warnings;
  use Attribute::Handlers;

  my %prefixes;

  sub import
  {
      my ($self, $ref)      = @_;
      my $package           = caller();
      $prefixes{ $package } = $ref;
  }

  sub gen_prefix
  {
      my $invalid_chars = "\0\r\n\f\b";

      my $prefix;

      for ( 1 .. 5 )
      {
          my $char_pos = int( rand( length( $invalid_chars ) ) );
          $prefix     .= substr( $invalid_chars, $char_pos, 1 );
      }

      return $prefix;
  }

  package UNIVERSAL;

  sub Private :ATTR
  {
      my ($package, $symbol, $referent, $attr, $data, $phase) = @_;

      my $name    = *{ $symbol }{NAME};
      my $newname = Class::HideMethods::gen_prefix( $package ) . $name;
      my @refs    = map { *$symbol{ $_ } } qw( HASH SCALAR ARRAY GLOB );
      *$symbol    = do { local *symbol };

      no strict 'refs';
      *{ $package . '::' . $newname } = $referent;
      *{ $package . '::' . $name    } = $_ for @refs;
      $prefixes{ $package }{ $name }  = $newname;
  }

  1;