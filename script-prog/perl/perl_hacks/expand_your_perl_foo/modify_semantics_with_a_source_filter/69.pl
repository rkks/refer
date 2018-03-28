  sub usage
  {
      if ($::VERBOSE)
      {
          print <<"END_USAGE";
              Usage: $0 [options] <infile> <outfile>

              Options:
                  -z       Zero tolerance on formatting errors
                  -o       Output overview only
                  -d       Debugging mode
              END_USAGE
      }
  }