#!/usr/bin/perl -w --
# Split text or binary files; jjohn 2/2002
use strict;
use Getopt::Std;

my %opts;
getopts("?b:f:hp:ts:", \%opts);

if( $opts{'?'} || $opts{'h'} || !-e $opts{'f'}){
  print <<USAGE;
$0 - split files in smaller ones

USAGE:
    $0 -b 1500 -f big_file -p part.

OPTIONS:

    -?       print this screen
    -h       print this screen
    -b <INT> split file into given byte size parts
    -f <TXT> the file to be split
    -p <TXT> each new file to begin with given text
    -s <INT> split file into given number of parts
USAGE
   exit;
}

my $infile;
open($infile, $opts{'f'}) or die "No file given to split\n";
binmode($infile);
my $infile_size = (stat $opts{'f'})[7];

my $block_size = 1;
if( $block_size = $opts{'b'} ){
  # chunk file into blocks

}elsif( my $total_parts = $opts{'s'} ){
  # chunk file into N parts
  $block_size = int ( $infile_size / $total_parts) + 1;

}else{
  die "Please indicate how to split file with -b or -s\n";
}

my $outfile_base = $opts{'p'} || 'part.';
my $outfile_ext = "aa";

my $offset = 0;
while( $offset < $infile_size ){
  my $buf;
  $offset += read $infile, $buf, $block_size;
  write_file($outfile_base, $outfile_ext++, \$buf);
}

#--- subs ---#
sub write_file {
  my($fname, $ext, $buf) = @_;

  my $outfile;
  open($outfile, ">$fname$ext") or die "can't open $fname$ext\n";
  binmode($outfile);
  my $wrote = syswrite $outfile, $$buf;
  my $size  = length($$buf);
  warn "WARN: wrote $wrote bytes instead of $size to $fname$ext\n"
    unless $wrote == $size;
}
