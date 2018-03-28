#! /usr/bin/perl --
##############################################################################
##   Filter script (Recommended) from Appendix C of "Perl Best Practices"   ##
##     Copyright (c) O'Reilly & Associates, 2005. All Rights Reserved.      ##
##  See: http://www.oreilly.com/pub/a/oreilly/ask_tim/2001/codepolicy.html  ##
##############################################################################

use Perl6::Slurp;
 
my $selection = slurp;
 
print <<"END_REPLACEMENT"
use Benchmark qw( cmpthese );
cmpthese -10, {
    $selection
};
END_REPLACEMENT

