C++ Template Callback Library
Copyright 1994 Rich Hickey

/* Permission to use, copy, modify, distribute and sell this software
 * for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Rich Hickey makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
*/

The library has seen widespread adoption. I get continuing correspondence
from people around the world using the library in various contexts. Thanks to
all for the generous support and feedback.

The only thing keeping more libraries from leveraging this library is the
inability of some compilers (one in particular) to compile it. I have done
the port myself so that obstacle is now removed.

The library can be found at:

http://ourworld.compuserve.com/HomePages/RichHickey

////////////////// 9/2/96 ///////////////////////////

Second public release of the library. The release consists of:

callback.hpp	//the library proper
callback.cpp	//source for comparison ops

cbsuite.cpp	//a non-exhaustive test suite

cbexam1.cpp	//examples
cbexam2.cpp
cbexam3.cpp
cbexam4.cpp

readme.txt	//this text

vc42bugs.cpp	//FYI


Satisfies the 2 most popular requests:

1) Added ==, != and < for functors so you can remove them from containers etc.
2) Ported to Microsoft's compiler (version 4.2)

I have still not made stored parameters publicly available since I haven't
figured out how to add them without reducing the efficiency of the library
(and no one is asking for them).

Still no known bugs - hopefuly I haven't introduced any.

Tested with Microsoft Visual C++ 4.2 and IBM Visual Age C++ 3.5
(should still work with Borland and Watcom and others)

If you are having problems with your compiler, check out the flags I added to
work around MSVC.

The article lives!
The original article documenting the rationale behind this library has been
included in a new book - C++ Gems, edited by Stan Lippman and published
by SIGS. 557 pages/softcover/ISBN 1-884842-10-0

///////////////// 1/30/95 /////////////////////////////

This is the first public release of this library.

This library provides a comprehensive, portable, type-safe, general
purpose mechanism for inter-component callbacks.  A complete rationale
and description of this mechanism appears in my article in the February
1995 issue of the C++ Report.  Users are strongly encouraged to read the
article before proceeding, as the library has minimal embedded
documentation at present.

If you are attempting a port to another compiler, make sure the suite
and examples compile and you can proceed with a good degree of certainty
that the library will perform correctly from then on.

The only new feature not covered in the article is a third flavor of
makeFunctor(), taking a pointer-to-member-function but _no object_. In
this case there must be at least one argument to the functor, and the
member function will be called on that argument, passing to it any
additional parameters. This feature is not covered in the suite yet, but
cbexam3.cpp provides a sample of its use.

The typedefs for STL are untested.

Planned Enhancements:

	A version that supports stored parameters is in the works.

Known bugs:

	None at present.

///////////////////////////////////////////////////////////////////

Please send bug reports, comments, etc to:

rich@rcsworks.com
hickeyr@ibm.net

I hope you find, as I do, that having a mechanism such as this greatly
increases the flexibility you have when designing systems in C++.

Enjoy.

Rich Hickey

