package Logger;
  
use strict;
  
our $OUTPUT_FH     = *STDERR;
our $REPORT_TIME   = 1;
our $REPORT_CALLER = 1;
  
sub log_message
{
    my ($package, $file, $line)  = caller(  );
    my $time                     = localtime(  );
  
    my $message                  = '';
    $message                    .= "[$time] " if $REPORT_TIME;
    $message                    .= shift;
    $message                    .= " from $package:$line in $file"
                                              if $REPORT_CALLER;
    $message                    .= "\n";
  
    write_message( $message );
}
  
sub write_message
{
    my $message = shift;
    print $OUTPUT_FH $message;
}
  
1;
