#!/usr/bin/perl

BEGIN { print "First!\n"  }
INIT  { print "Fourth!\n" }
CHECK { print "Third!\n"  }
CHECK { print "Second!\n" }
INIT  { print "Fifth!\n"  }
