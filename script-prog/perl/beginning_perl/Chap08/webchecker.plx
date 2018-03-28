#!/usr/bin/perl
# webchecker.plx
use warnings;
use strict;
my %seen;

print "Web Checker, version 1.\n";
die "Usage: $0 <starting point> <site base>\n"
   unless @ARGV == 2;

my ($start, $base) = @ARGV;
$base .= "/" unless $base=~m|/$|;

die "$start appears not to be in $base\n"
   unless in_our_site($start);
traverse($start);

sub traverse {
   my $url = shift;
   $url =~ s|/$|/index.html|;
   return if $seen{$url}++;        # Break circular links
   my $page = get($url);
   if ($page) {
      print "Link OK : $url\n";
   } else {
      print "Link dead : $url\n";
      return;                    # Terminating condition : if dead.
   }
   return unless in_our_site($url); # Terminating condition : if external.
   my @links = extract_links($page, $url);
   return unless @links;            # Terminating condition : no links
   for my $link (@links) {
      traverse($link) # Recurse
   }
}

sub in_our_site {
   my $url = shift;
   return index($url, $base) == 0;
}

sub get {
   my $what = shift;
   sleep 5; # Be friendly
   return `lynx -source $what`;
}

sub extract_links{
   my ($page, $url) = @_;
   my $dir = $url;
   my @links;
   $dir =~ s|(.*)/.*?$|$1|; 
   for (@links = ($page=~/<A HREF=["']?([^\s"'>]+)["']?/gi)) {
      $_ = $base.$_ if s|^/||;
      $_ = $dir."/".$_ if !/^(ht|f)tp:/;
   }
   return @links;
}
