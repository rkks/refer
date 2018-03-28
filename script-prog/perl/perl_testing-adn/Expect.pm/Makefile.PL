use ExtUtils::MakeMaker;

use Config;

print "Be sure to read the file 'Changes' if you are upgrading from 1.01.\n";
print "Behavior of pattern matching in expect() has changed significantly.\n";
print "You might also want to check out soft_close() and hard_close().\n";

WriteMakefile(
    'NAME'	    => 'Expect',
    'VERSION_FROM'  => "Expect.pm",
);
