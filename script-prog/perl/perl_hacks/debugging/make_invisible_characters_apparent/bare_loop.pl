  while (<$file_list>)
  {
      chomp;
      next unless -e $_;
      process_file( $_ );
  }
