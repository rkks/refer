  sub install_nodemethods
  {
      my $dbh = shift;

      my $sth = $dbh->prepare(<<'END_SQL');
  SELECT
      types.title AS class, methods.title AS method, nodemethod.code AS code
  FROM
      nodemethod
  LEFT JOIN
      node AS types ON types.node_id = nodemethod.supports_nodetype
  END_SQL

      $sth->execute();

      # ... do something with the data
  }