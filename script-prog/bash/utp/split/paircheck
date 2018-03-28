#!/bin/gawk -f
# paircheck - be sure each <UL> tag has a matching </UL>

BEGIN {
    IGNORECASE = 1
    inList = 0
    LSlineno = 0
    LElineno = 0
    prevFile = ""
}
# if more than one file, check for unclosed list in first file
FILENAME != prevFile {
    if (inList)
     printf ("%s: found <UL> at line %d without </UL> before end of file\n",
            prevFile, LSlineno)
    inList = 0
    prevFile = FILENAME
}
# match <UL> and see if we are in list
/^<UL>/ {
    if (inList) {
        printf("%s: nested list starts: line %d and %d\n",
            FILENAME, LSlineno, FNR)
    }
    inList = 1
    LSlineno = FNR
}
/^<\/UL>/ {
    if (! inList)
        printf("%s: too many list ends: line %d and %d\n",
            FILENAME, LElineno, FNR)
    else
        inList = 0
    LElineno = FNR
}
# this catches end of input
END {
    if (inList)
        printf ("%s: found <UL> at line %d without </UL> before end of file\n",
            FILENAME, LSlineno)
}
