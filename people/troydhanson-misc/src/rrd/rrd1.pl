#!/usr/bin/perl
my $f = "rrd1.rrd";
unlink $f if [ -f $f ];
my $now = time();
my $o;
# one data 'source' which updates every second (-s 1)
# we create two archives from it: the first records every
# individual value for 60 measurements (...1:60), while
# the second records 100 values each averaged over 60 measurements
$o = `rrdtool create $f -b $now -s 1 DS:source:GAUGE:120:-1:1 RRA:LAST:0.5:1:60 RRA:AVERAGE:0.5:60:100`;
die "$o\n" if $o;

# generate up some data
my %gauge;
my $max = 36;
for my $t (0..$max) {
  my $radian = ($t * 1.0 / $max) * (2*3.14159);
  $gauge{$t} = cos $radian;
}

# put it into the rrdtool
print "now is $now\n";
for my $t (sort {$a <=> $b} keys %gauge) {
  my $ts = $now + $t + 1;
  #print "rrdtool update $f $ts:$gauge{$t}", "\n";
  $o = `rrdtool update $f $ts:$gauge{$t}`;
  die "$o\n" if $o;
}

# make a picture
my $end = $now + $max;
$o = `rrdtool graph rrd1.png -s $now -e $end DEF:thegauge=$f:source:LAST LINE2:thegauge#ff0000`;
die "$o\n" if $o;
