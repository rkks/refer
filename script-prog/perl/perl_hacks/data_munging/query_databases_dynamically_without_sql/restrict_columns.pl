  sub restrict_columns
  {
      my ($self, $user, $table, $columns) = @_;
      my $user_columns                    = $user->get_columns_for( $table );
      return [ grep { exists $user_columns->{ $_ } } ] @$columns;
  }