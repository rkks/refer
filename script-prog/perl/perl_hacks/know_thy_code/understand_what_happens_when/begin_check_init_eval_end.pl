#!/usr/bin/perl

BEGIN { print "First!\n"  }
INIT  { print "Fourth!\n" }
CHECK { print "Third!\n"  }
CHECK { print "Second!\n" }
INIT  { print "Fifth!\n"  }
eval <<END_EVAL;
BEGIN { print "BEGIN in eval\n!" }
CHECK { print "CHECK in eval\n!" }
INIT  { print "INIT in eval\n!"  }
END   { print "Sixth!\n"         }
END_EVAL
END   { print "Seventh!\n"       }
END   { print "Eighth!\n"        }
