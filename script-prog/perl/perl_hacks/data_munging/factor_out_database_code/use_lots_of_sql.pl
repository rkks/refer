  use Lots::Of::SQL;

  sub install_nodemethods
  {
      my $dbh = shift;

      my $sth = $dbh->prepare( select_nodemethod_attributes() );
      $sth->execute();

      # ... do something with the data
  }