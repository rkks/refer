#!/usr/bin/perl

BEGIN { print "First!\n"  }
CHECK { print "Third!\n"  }
CHECK { print "Second!\n" }
