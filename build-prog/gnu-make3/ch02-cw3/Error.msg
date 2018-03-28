I was attempting to use the -l link flag feature in a prerequisite and
discovered a surpising feature.  make will not match a -lxx
prerequisite with a libxx.a target:

vpath %.a .
count_words: count_words.o -lcounter
libcounter.a: libcounter.a(lexer.o)

When run yields:

make
gcc    -c -o count_words.o count_words.c
make: *** No rule to make target `-lcounter', needed by `count_words'.  Stop.

$ make --version
GNU Make 3.80
Copyright (C) 2002  Free Software Foundation, Inc.

Is this expected behavior?

Thanks,
Robert
