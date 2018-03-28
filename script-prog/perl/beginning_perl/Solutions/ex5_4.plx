#!/usr/bin/perl
# ex05_04.plx
use warnings;
use strict;

$_ = <<EOF;
      <put your data in here>
EOF

my ($count, $swaps, $done) = (0,0,0);

until ($done) {
      m|^(.+\n){$count}|g;      # match first '$count' lines in $_

      if ( m|\G(.+)\n(.+)| ) {      # try matching the next pair
         if ($2 lt $1) {      # if they're in the wrong order
            s|\G(.+)\n(.+)|$2\n$1|; # swap 'em round
            $swaps++;
            } else {      # otherwise,
            pos()=0;      # reset the \G boundary for $_
            }
         $count++;
      } else {               
         $done = 1 if ($swaps == 0);   # done if there were no swaps
         ($count, $swaps) = (0,0);   # reset for next time around
      }
}
print;

