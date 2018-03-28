  package My::Filter;
  use Filter::Simple;

  FILTER_ONLY code => sub
  {
      # The code from any program that uses this module
      # is passed into this subroutine in $_.
      # Whatever is in $_ at the end of this subroutine
      # becomes the source code that the compiler eventually sees.
  };

  1;
