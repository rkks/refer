  while (<$file_list>)
  {
      warn "Processing '$_'";
      next unless -e $_;
      process_file( $_ );
  }
