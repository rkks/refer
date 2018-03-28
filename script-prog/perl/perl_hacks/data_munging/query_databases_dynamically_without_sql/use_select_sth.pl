  my $table   = 'users';
  my $columns = [qw( login_name last_accessed_on email_address )];
  my $where   = { signup_date => { '>=', '20050101' } };
  my $sth     = $model->get_select_sth( $table, $columns, $where );