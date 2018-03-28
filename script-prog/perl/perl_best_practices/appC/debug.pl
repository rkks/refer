#! /usr/bin/perl --
##############################################################################
##   Filter script (Recommended) from Appendix C of "Perl Best Practices"   ##
##     Copyright (c) O'Reilly & Associates, 2005. All Rights Reserved.      ##
##  See: http://www.oreilly.com/pub/a/oreilly/ask_tim/2001/codepolicy.html  ##
##############################################################################

print 'use Data::Dumper qw( Dumper );\nwarn Dumper [ ', <>, ' ]';

