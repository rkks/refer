  use SQL::Abstract;

  sub get_select_sth
  {
      my ($self, $table, $columns, $where) = @_;

      my $sql           = SQL::Abstract->new();
      my ($stmt, @bins) = $sql->select( $table, $columns, $where );
      my $sth           = $self->get_dbh()->prepare( $stmt );

      $sth->execute();
      return $sth;
  }