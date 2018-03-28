#!/usr/bin/perl

# Script to locate strings inside a specified include file and it's 
# includes, up to a maximum coded level of recursion.
# Written by Tyler Montbriand in November 2005.  Freeware.

# Print usage information and exit
sub show_usage();
# Add an include file to the end of @files
sub add_include_file($);

# The list of include files to process
my @files;
# Hash to remember files we've done already
my %checked=();
my ($str,$tmp,$n,$dcount)=(shift,shift,0,3);
defined($str) || show_usage();
defined($tmp) || show_usage();

# Add include files until we run out of paramaters
while(defined($tmp))
{
  add_include_file($tmp) || print STDERR "Warning -- could not find $tmp\n";
  $tmp=shift;
}

# Loop while the list is not empty, and we've done < $dcount recursions
while((@files > 0 ) && ($dcount > 0))
{
  # The number of files to remove from the front of the list this recursion
  $n=@files;

  # Process that many, and only that many files.
  while($n > 0)
  {
    $fname=shift @files;

    if(open(INCFILE,$fname))
    {
      # Marked file as checked so we'll skip it next time
      $checked{$fname}=1;

      while(defined($tmp=<INCFILE>))
      {
        if($tmp =~ /^#[ \t]*include[ \t]*(["<][^"<>]*[">"])/)
        {  add_include_file($1);  }

        if($tmp =~ $str)
        {
          print STDERR "\a$fname:\t$tmp";
        }

      }
      close(INCFILE);
    }
    else { print STDERR "Warning!  Couldn't open $fname\n"; }
    $n--;
  }

  $dcount--;
}

exit 0;

sub add_include_file($)
{
  my $file=shift;

  if($file =~ /^<([^<>]+)>$/)   {  $file = "/usr/include/$1";  }
  elsif($file =~ /^"([^"]+)"$/) {  $file = $1;                 }

  if(defined($checked{$fname}))
  {
    # No error, but don't do anything
    return(1);
  }

  if(-f $file)
  {
    push @files,$file;
    $checked{$fname}=1;
    return(1);
  }
  else
  {
    return(0);
  }
}

sub show_usage()
{
  printf STDERR "Usage:  %s searchstr includefile [include2 ...]\n",$0;
  exit 1;
}
