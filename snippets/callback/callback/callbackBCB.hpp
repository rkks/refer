//**************** callbackBCB.hpp **********************
// Copyright 1994 Rich Hickey
/* Permission to use, copy, modify, distribute and sell this software
* for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Rich Hickey makes no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*/

// 10/04/2001 Martin Zimmermann (zimmermann@physik.uni-kiel.de)
// The code didn't work with Borland Compiler 5.5X . The Compiler was not able
// to resolve TRT(Callee::*)(....) , when passed as a function argument. This is
// obviously a BUG in the compiler (Code was successfully compiled on 6
// different ANSI compilers, but only Borland failed)
// The encapsulation in a seperate struct solves the problem. The code still
// should be portable. Compilation has been successful on MSVC++ 6.0 and
// gcc2.95.X without any changes in the code

// 08/31/96 Rich Hickey
// Added ==, != and <
//  They are not inline, source is in file callback.cpp
//  Note: You must compile and link in callback.obj if you use them
// C++ doesn't allow ptr-to-func to void * anymore -> changed to void (*)(void)
// Added compiler workarounds for MS VC++ 4.2
// Prefixed all macros with CB
// Note: derivation from CB_FtorBase is now public, and access functions
// (for func, callee etc) are provided >>for implementation use only<<

// 06/12/94 Rich Hickey
// 3rd major revision
// Now functors are concrete classes, and should be held by value
// Virtual function mechanism removed
// Generic MakeFtor() mechanism added for building functors
// from both stand-alone functions and object/ptr-to-mem-func pairs

#ifndef CALLBACK_LIB_HICKEY
#define CALLBACK_LIB_HICKEY


/*
To use:

  If you wish to build a component that provides/needs a callback, simply
  specify and hold a CB_Ftor of the type corresponding to the args
  you wish to pass and the return value you need. There are 10 Functors
  from which to choose:

	CB_Ftor0
	CB_Ftor1<P1>
	CB_Ftor2<P1,P2>
	CB_Ftor3<P1,P2,P3>
	CB_Ftor4<P1,P2,P3,P4>
	CB_Ftor0wRet<RT>
	CB_Ftor1wRet<P1,RT>
	CB_Ftor2wRet<P1,P2,RT>
	CB_Ftor3wRet<P1,P2,P3,RT>
	CB_Ftor4wRet<P1,P2,P3,P4,RT>

These are parameterized by their args and return value if any. Each has
a default ctor and an operator() with the corresponding signature.

They can be treated and used just like ptr-to-functions.

If you want to be a client of a component that uses callbacks, you
create a CB_Ftor by calling MakeFtor().

There are three flavors of MakeFtor - you can create a functor from:

a ptr-to-stand-alone function
an object and a pointer-to-member function.
a pointer-to-member function (which will be called on first arg of functor)

Note: this last was not covered in the article - see CBEXAM3.CPP

The current iteration of this library requires you pass MakeFtor()
a dummy first argument of type ptr-to-the-Functor-type-you-want-to-create.
Simply cast 0 to provide this argument:

MakeFtor((target-functor*)0,ptr-to-function)
MakeFtor((target-functor*)0,reference-to-object,ptr-to-member-function)
MakeFtor((target-functor*)0,ptr-to-member-function)

Future versions will drop this requirement once member templates are
available.

The functor system is 100% type safe. It is also type flexible. You can
build a functor out of a function that is 'type compatible' with the
target functor - it need not have an exactly matching signature. By
type compatible I mean a function with the same number of arguments, of
types reachable from the functor's argument types by implicit conversion.
The return type of the function must be implicitly convertible to the
return type of the functor. A functor with no return can be built from a
function with a return - the return value is simply ignored.
(See ethel example below)

All the correct virtual function behavior is preserved. (see ricky
example below).

If you somehow try to create something in violation
of the type system you will get a compile-time or template-instantiation-
time error.

The CB_Ftor base class and translator
classes are artifacts of this implementation. You should not write
code that relies upon their existence. Nor should you rely on the return
value of MakeFtor being anything in particular.

All that is guaranteed is that the Functor classes have a default ctor,
a ctor that can accept 0 as an initializer,
an operator() with the requested argument types and return type, an
operator that will allow it to be evaluated in a conditional (with
'true' meaning the functor is set, 'false' meaning it is not), and that
Functors can be constructed from the result of MakeFtor(), given
you've passed something compatible to MakeFtor(). In addition you
can compare 2 functors with ==, !=, and <. 2 functors with the same
'callee' (function, object and/or ptr-to-mem-func) shall compare
equal. op < forms an ordering relation across all callee types -> the
actual order is not meaningful or to be depended upon.

/////////////////////// BEGIN Example 1 //////////////////////////
#include <iostream.h>
#include "callback.hpp"

//do5Times() is a function that takes a functor and invokes it 5 times

void do5Times(const CB_Ftor1<int> &doIt)
{
for(int i=0;i<5;i++)
doIt(i);
}

//Here are some standalone functions

void fred(int i){cout << "fred: " << i<<endl;}
int ethel(long l){cout << "ethel: " << l<<endl;return l;}

//Here is a class with a virtual function, and a derived class

class B{
public:
virtual void ricky(int i)
{cout << "B::ricky: " << i<<endl;}
};

class D:public B{
public:
void ricky(int i)
{cout << "D::ricky: " << i<<endl;}
};

void main()
{
//create a typedef of the functor type to simplify dummy argument
typedef CB_Ftor1<int> *FtorType;

CB_Ftor1<int> ftor;   //a functor variable

//make a functor from ptr-to-function
ftor = MakeFtor((FtorType)0,fred);
do5Times(ftor);
//note ethel is not an exact match - ok, is compatible
ftor = MakeFtor((FtorType)0,ethel);
do5Times(ftor);

//create a D object to be a callback target
D myD;
//make functor from object and ptr-to-member-func
ftor = MakeFtor((FtorType)0,myD,&B::ricky);
do5Times(ftor);
}
/////////////////////// END of example 1 //////////////////////////

/////////////////////// BEGIN Example 2 //////////////////////////
#include <iostream.h>
#include "callback.hpp"

//Button is a component that provides a functor-based
//callback mechanism, so you can wire it up to whatever you wish

class Button{
public:
//ctor takes a functor and stores it away in a member

Button(const CB_Ftor0 &uponClickDoThis):notify(uponClickDoThis)
{}
void click()
{
//invoke the functor, thus calling back client
notify();
}
private:
//note this is a data member with a verb for a name - matches its
//function-like usage
CB_Ftor0 notify;
};

class CDPlayer{
public:
void play()
{cout << "Playing"<<endl;}
void stop()
{cout << "Stopped"<<endl;}
};

void main()
{
CDPlayer myCD;
Button playButton(MakeFtor((CB_Ftor0*)0,myCD,&CDPlayer::play));
Button stopButton(MakeFtor((CB_Ftor0*)0,myCD,&CDPlayer::stop));
playButton.click();   //calls myCD.play()
stopButton.click();  //calls myCD.stop()
}
/////////////////////// END of example 2 //////////////////////////

*/

//******************************************************************
///////////////////////////////////////////////////////////////////*
//WARNING - no need to read past this point, lest confusion ensue. *
//Only the curious need explore further - but remember				 *
//about that cat!																	 *
///////////////////////////////////////////////////////////////////*
//******************************************************************

//////////////////////////////
// COMPILER BUG WORKAROUNDS:

// MS VC++ 4.2 still has many bugs relating to templates
// This version works around them as best I can - however note that
// MS will allow 'void (T::*)()const' to bind to a non-const member function
// of T. In addition, they do not support overloading template functions
// based on constness of ptr-to-mem-funcs.
// When _MSC_VER is defined I provide only the const versions,however it is on
// the user's head, when calling MakeFtor with a const T, to make sure
// that the pointed-to member function is also const since MS won't enforce it!

// Other than that the port is completely functional under VC++ 4.2

// One MS bug you may encounter during _use_ of the callbacks:
// If you pass them by reference you can't call op() on the reference
// Workaround is to pass by value.

/*
// MS unable to call operator() on template class reference
template <class T>
class Functor{
public:
void operator()(T t)const{};
};

  void foo(const Functor<int> &f)
  {
  f(1);	//error C2064: term does not evaluate to a function
  
	//works when f is passed by value
	}
*/

// Note: if you are porting to another compiler that is having trouble you
// can try defining some of these flags as well:



//#if defined(_MSC_VER)	
#if defined(WIN32)	// BP
	#define CB_CANT_PASS_MEMFUNC_BY_REFERENCE	//like it says
//	#define CB_CANT_OVERLOAD_ON_CONSTNESS		//of mem funcs
	#define CB_CANT_INIT_REFERENCE_CTOR_STYLE	//int i;int &ir(i); //MS falls down
	#define CB_WONT_PERFORM_PTR_CONVERSION		//of 0 to ptr-to-any-type
#endif


// Don't touch this stuff
#if defined(CB_CANT_PASS_MEMFUNC_BY_REFERENCE)
	#define CB_CONST_REF
#else
	#define CB_CONST_REF const &
#endif

#if defined(CB_CANT_INIT_REFERENCE_CTOR_STYLE)
	#define CB_CTOR_STYLE_INIT =
#else
	#define CB_CTOR_STYLE_INIT
#endif

#if defined(CB_WONT_PERFORM_PTR_CONVERSION)
	#define CB_DUMMY_INIT int
#else
	#define CB_DUMMY_INIT DummyInit *
#endif


////////////////////////////// THE CODE //////////////////////////


//change these when your compiler gets bool
typedef bool CB_BOOL;
enum {CB_FALSE ,CB_TRUE};

#include <string.h> //for memstuff
#include <stddef.h> //for size_t

//typeless representation of a function and optional object

class CB_FtorBase{
	public:
		//Note: ctors are protected
		
		//for evaluation in conditionals - can the functor be called?
		operator CB_BOOL()const{return callee||func;}
		
		//Now you can put them in containers _and_ remove them!
		//Source for these 3 is in callback.cpp
		friend CB_BOOL
			operator==(const CB_FtorBase &lhs,const CB_FtorBase &rhs);
		friend CB_BOOL
			operator!=(const CB_FtorBase &lhs,const CB_FtorBase &rhs);
		friend CB_BOOL
			operator<(const CB_FtorBase &lhs,const CB_FtorBase &rhs);
		
		//The rest below for implementation use only !
		
		class DummyInit{
			};
		
		typedef void (CB_FtorBase::*PMemFunc)();
		typedef void (*PFunc)();
		enum {MEM_FUNC_SIZE = sizeof(PMemFunc)};
		
		PFunc	getFunc() const {return func;}
		void *getCallee() const {return callee;}
		const char *getMemFunc() const {return memFunc;}

			protected:
				////////////////////////////////////////////////////////////////
				// Note: this code depends on all ptr-to-mem-funcs being same size
				// If that is not the case then make memFunc as large as largest
				////////////////////////////////////////////////////////////////
				union{
					PFunc func;
					char memFunc[MEM_FUNC_SIZE];
					};
				void *callee;

				CB_FtorBase():callee(0),func(0){}
				CB_FtorBase(const void *c,PFunc f, const void *mf,size_t sz):
				callee((void *)c)
					{
					if(c)	//must be callee/memfunc
						{
						memcpy(memFunc,mf,sz);
						if(sz<MEM_FUNC_SIZE)	//zero-out the rest, if any, so comparisons work
							{
							memset(memFunc+sz,0,MEM_FUNC_SIZE-sz);
							}
						}
					else	//must be ptr-to-func
						{
						func = f;
						}
					}
	};


/* **************************************************************
*		Changed 10/04/2001 by Martin Zimmermann
*			zimmermann@physik.uni-kiel.de
*
*		Introduction of structures for the templated pointer
*		to member-functions (otherwise it wouldn't work with
*		bcc32 5.5 (there is a BUG in bcc32 5.5X !!!!!!! )
*		No ANSI support ...
****************************************************************/

/* *****************************
*		with const
*******************************/

// TRT (CallType::*)() <--> RebindMemTrans0c<TRT,CallType>::MemFunc
template <class TRT,class CallType>
struct RebindMemTrans0c
{
	typedef TRT (CallType::*MemFunc)()const;
};

// TRT (CallType::*)(TP1) <--> RebindMemTrans1c<TRT,CallType,TP1>::MemFunc
template <class TRT,class CallType, class TP1>
struct RebindMemTrans1c
{
	typedef TRT (CallType::*MemFunc)(TP1)const;
};

// TRT (CallType::*)(TP1,TP2) <--> RebindMemTrans2c<TRT,CallType,TP1,TP2>::MemFunc
template <class TRT,class CallType, class TP1, class TP2>
struct RebindMemTrans2c
{
	typedef TRT (CallType::*MemFunc)(TP1,TP2)const;
};

// TRT (CallType::*)(TP1,TP2,TP3) <--> <--> RebindMemTrans3c<TRT,CallType,TP1,TP2,TP3>::MemFunc
template <class TRT,class CallType, class TP1, class TP2,class TP3>
struct RebindMemTrans3c
{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3)const;
};

// TRT (CallType::*)(TP1,TP2,TP3,TP4) <--> <--> RebindMemTrans4c<TRT,CallType,TP1,TP2,TP3,TP4>::MemFunc
template <class TRT,class CallType, class TP1, class TP2,class TP3,class TP4>
struct RebindMemTrans4c
{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4)const;
};

/* *****************************
*		without const
*******************************/

// TRT (CallType::*)() <--> RebindMemTrans0<TRT,CallType>::MemFunc
template <class TRT,class CallType>
struct RebindMemTrans0
{
	typedef TRT (CallType::*MemFunc)();
};

// TRT (CallType::*)(TP1) <--> RebindMemTrans1<TRT,CallType,TP1>::MemFunc
template <class TRT,class CallType, class TP1>
struct RebindMemTrans1
{
	typedef TRT (CallType::*MemFunc)(TP1);
};

// TRT (CallType::*)(TP1,TP2) <--> RebindMemTrans2<TRT,CallType,TP1,TP2>::MemFunc
template <class TRT,class CallType, class TP1, class TP2>
struct RebindMemTrans2
{
	typedef TRT (CallType::*MemFunc)(TP1,TP2);
};

// TRT (CallType::*)(TP1,TP2,TP3) <--> <--> RebindMemTrans3<TRT,CallType,TP1,TP2,TP3>::MemFunc
template <class TRT,class CallType, class TP1, class TP2,class TP3>
struct RebindMemTrans3
{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3);
};

// TRT (CallType::*)(TP1,TP2,TP3,TP4) <--> <--> RebindMemTrans4<TRT,CallType,TP1,TP2,TP3,TP4>::MemFunc
template <class TRT,class CallType, class TP1, class TP2,class TP3,class TP4>
struct RebindMemTrans4
{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4);
};



/*****************************************************************
*		End of changes. Now it also works with bbc5.5X !! :)))
*****************************************************************/


/************************* no arg - no return *******************/
class CB_Ftor0:public CB_FtorBase{
	public:
		CB_Ftor0(CB_DUMMY_INIT = 0){}
		void operator()()const
			{
			thunk(*this);
			}
	protected:
		typedef void (*Thunk)(const CB_FtorBase &);
		CB_Ftor0(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		CB_FtorBase(c,f,mf,sz),thunk(t){}
	private:
		Thunk thunk;
	};

template <class Callee, class MemFunc>
class CB_MemTrans0:public CB_Ftor0{
	public:
		CB_MemTrans0(Callee &c,const MemFunc &m):
		  CB_Ftor0(thunk,&c,0,&m,sizeof(MemFunc)){}
		  static void thunk(const CB_FtorBase &ftor)
			  {
			  Callee *callee = (Callee *)ftor.getCallee();
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  (callee->*memFunc)();
			  }
	};

template <class Func>
class CB_FuncTrans0:public CB_Ftor0{
	public:
		CB_FuncTrans0(Func f):CB_Ftor0(thunk,0,(PFunc)f,0,0){}
		static void thunk(const CB_FtorBase &ftor)
			{
			(Func(ftor.getFunc()))();
			}
	};


#ifndef CB_CANT_OVERLOAD_ON_CONSTNESS
template <class Callee,class TRT,class CallType>
inline CB_MemTrans0<Callee,RebindMemTrans0<TRT,CallType>::MemFunc>
MakeFtor(CB_Ftor0 *,Callee &c,TRT (CallType::* CB_CONST_REF f)())
	{
	typedef TRT (CallType::*MemFunc)();
	return CB_MemTrans0<Callee,MemFunc>(c,f);
	}
#endif


template <class Callee,class TRT,class CallType>
inline CB_MemTrans0<const Callee,RebindMemTrans0c<TRT,CallType>::MemFunc >
MakeFtor(CB_Ftor0 *,const Callee &c,
		  TRT (CallType::* CB_CONST_REF f)()const)
	{
	typedef TRT (CallType::*MemFunc)()const;
	return CB_MemTrans0<const Callee,MemFunc>(c,f);
	}

template <class TRT>
inline CB_FuncTrans0<TRT (*)()>
MakeFtor(CB_Ftor0 *,TRT (*f)())
	{
	return CB_FuncTrans0<TRT (*)()>(f);
	}

/************************* no arg - with return *******************/
template <class RT>
class CB_Ftor0wRet:public CB_FtorBase{
	public:
		CB_Ftor0wRet(CB_DUMMY_INIT = 0){}
		RT operator()()const
			{
			return (thunk(*this));	
			}
	protected:
		typedef RT (*Thunk)(const CB_FtorBase &);
		CB_Ftor0wRet(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		CB_FtorBase(c,f,mf,sz),thunk(t){}
	private:
		Thunk thunk;
	};

template <class RT,class Callee, class MemFunc>
class CB_MemTrans0wRet:public CB_Ftor0wRet<RT>{
	public:
		CB_MemTrans0wRet(Callee &c,const MemFunc &m):
		  CB_Ftor0wRet<RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
		  static RT thunk(const CB_FtorBase &ftor)
			  {
			  Callee *callee = (Callee *)ftor.getCallee();
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  return ((callee->*memFunc)());
			  }
	};

template <class RT,class Func>
class CB_FuncTrans0wRet:public CB_Ftor0wRet<RT>{
	public:
		CB_FuncTrans0wRet(Func f):CB_Ftor0wRet<RT>(thunk,0,(PFunc)f,0,0){}
		static RT thunk(const CB_FtorBase &ftor)
			{
			return (Func(ftor.getFunc()))();
			}
	};


#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class RT,class Callee,class TRT,class CallType>
inline CB_MemTrans0wRet<RT,Callee,RebindMemTrans0<TRT,CallType>::MemFunc>
MakeFtor(CB_Ftor0wRet<RT>*,Callee &c,TRT (CallType::* CB_CONST_REF f)())
	{
	typedef TRT (CallType::*MemFunc)();
	return CB_MemTrans0wRet<RT,Callee,MemFunc>(c,f);
	}
#endif


template <class RT,class Callee,class TRT,class CallType>
inline CB_MemTrans0wRet<RT,const Callee,RebindMemTrans0c<TRT,CallType>::MemFunc >
MakeFtor(CB_Ftor0wRet<RT>*,const Callee &c,
		  TRT (CallType::* CB_CONST_REF f)()const)
	{
	typedef TRT (CallType::*MemFunc)()const;
	return CB_MemTrans0wRet<RT,const Callee,MemFunc>(c,f);
	}

template <class RT,class TRT>
inline CB_FuncTrans0wRet<RT,TRT (*)()>
MakeFtor(CB_Ftor0wRet<RT>*,TRT (*f)())
	{
	return CB_FuncTrans0wRet<RT,TRT (*)()>(f);
	}

/************************* one arg - no return *******************/
template <class P1>
class CB_Ftor1:public CB_FtorBase{
	public:
		CB_Ftor1(CB_DUMMY_INIT = 0){}
		void operator()(P1 p1)const
			{
			thunk(*this,p1);
			}
		//for STL
		typedef P1 argument_type;
		typedef void result_type;
	protected:
		typedef void (*Thunk)(const CB_FtorBase &,P1);
		CB_Ftor1(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		CB_FtorBase(c,f,mf,sz),thunk(t){}
	private:
		Thunk thunk;
	};

template <class P1,class Callee, class MemFunc>
class CB_MemTrans1:public CB_Ftor1<P1>{
	public:
		CB_MemTrans1(Callee &c,const MemFunc &m):
		  CB_Ftor1<P1>(thunk,&c,0,&m,sizeof(MemFunc)){}
		  static void thunk(const CB_FtorBase &ftor,P1 p1)
			  {
			  Callee *callee = (Callee *)ftor.getCallee();
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  (callee->*memFunc)(p1);
			  }
	};


template <class P1,class Func>
class CB_FuncTrans1:public CB_Ftor1<P1>{
	public:
		CB_FuncTrans1(Func f):CB_Ftor1<P1>(thunk,0,(PFunc)f,0,0){}
		static void thunk(const CB_FtorBase &ftor,P1 p1)
			{
			(Func(ftor.getFunc()))(p1);
			}
	};


#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class Callee,class TRT,class CallType,class TP1>
inline CB_MemTrans1<P1,Callee,RebindMemTrans1<TRT,CallType,TP1>::MemFunc>
MakeFtor(CB_Ftor1<P1>*,Callee &c,TRT (CallType::* CB_CONST_REF f)(TP1))
	{
	typedef TRT (CallType::*MemFunc)(TP1);
	return CB_MemTrans1<P1,Callee,MemFunc>(c,f);
	}
#endif


template <class P1,class Callee,class TRT,class CallType,class TP1>
inline CB_MemTrans1<P1,const Callee,RebindMemTrans1c<TRT,CallType,TP1>::MemFunc>
MakeFtor(CB_Ftor1<P1>*,const Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1)const;
	return CB_MemTrans1<P1,const Callee,MemFunc>(c,f);
	}

template <class P1,class TRT,class TP1>
inline CB_FuncTrans1<P1,TRT (*)(TP1)>
MakeFtor(CB_Ftor1<P1>*,TRT (*f)(TP1))
	{
	return CB_FuncTrans1<P1,TRT (*)(TP1)>(f);
	}

template <class P1,class MemFunc>
class CB_ClassTrans1:public CB_Ftor1<P1>{
	public:
		CB_ClassTrans1(const MemFunc &m):
		  CB_Ftor1<P1>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
		  static void thunk(const CB_FtorBase &ftor,P1 p1)
			  {
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  (p1.*memFunc)();
			  }
	};


#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class TRT,class CallType>
inline CB_ClassTrans1<P1,RebindMemTrans0<TRT,CallType>::MemFunc>
MakeFtor(CB_Ftor1<P1>*,TRT (CallType::* CB_CONST_REF f)())
	{
	typedef TRT (CallType::*MemFunc)();
	return CB_ClassTrans1<P1,MemFunc>(f);
	}
#endif


template <class P1,class TRT,class CallType>
inline CB_ClassTrans1<P1,RebindMemTrans0c<TRT,CallType>::MemFunc >
MakeFtor(CB_Ftor1<P1>*,TRT (CallType::* CB_CONST_REF f)()const)
	{
	typedef TRT (CallType::*MemFunc)()const;
	return CB_ClassTrans1<P1,MemFunc>(f);
	}


/************************* one arg - with return *******************/
template <class P1,class RT>
class CB_Ftor1wRet:public CB_FtorBase{
	public:
		CB_Ftor1wRet(CB_DUMMY_INIT = 0){}
		RT operator()(P1 p1)const
			{
			return (thunk(*this,p1));
			}
		//for STL
		typedef P1 argument_type;
		typedef RT result_type;
	protected:
		typedef RT (*Thunk)(const CB_FtorBase &,P1);
		CB_Ftor1wRet(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		CB_FtorBase(c,f,mf,sz),thunk(t){}
	private:
		Thunk thunk;
	};

template <class P1,class RT,class Callee, class MemFunc>
class CB_MemTrans1wRet:public CB_Ftor1wRet<P1,RT>{
	public:
		CB_MemTrans1wRet(Callee &c,const MemFunc &m):
		  CB_Ftor1wRet<P1,RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1)
			  {
			  Callee *callee = (Callee *)ftor.getCallee();
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  return ((callee->*memFunc)(p1));
		  }
	};

template <class P1,class RT,class Func>
class CB_FuncTrans1wRet:public CB_Ftor1wRet<P1,RT>{
	public:
		CB_FuncTrans1wRet(Func f):
		  CB_Ftor1wRet<P1,RT>(thunk,0,(PFunc)f,0,0){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1)

			  {
			  return (Func(ftor.getFunc()))(p1);
			  }
	};


#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class RT,
class Callee,class TRT,class CallType,class TP1>
inline CB_MemTrans1wRet<P1,RT,Callee,
	RebindMemTrans1<TRT,CallType,TP1>::MemFunc>
MakeFtor(CB_Ftor1wRet<P1,RT>*,Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1))
	{
	typedef TRT (CallType::*MemFunc)(TP1);
	return CB_MemTrans1wRet<P1,RT,Callee,MemFunc>(c,f);
	}
#endif


template <class P1,class RT,
class Callee,class TRT,class CallType,class TP1>
inline CB_MemTrans1wRet<P1,RT,
const Callee, RebindMemTrans1c<TRT,CallType,TP1>::MemFunc>
MakeFtor(CB_Ftor1wRet<P1,RT>*,
		  const Callee &c,TRT (CallType::* CB_CONST_REF f)(TP1)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1)const;
	return CB_MemTrans1wRet<P1,RT,const Callee,MemFunc>(c,f);
	}

template <class P1,class RT,class TRT,class TP1>
inline CB_FuncTrans1wRet<P1,RT,TRT (*)(TP1)>
MakeFtor(CB_Ftor1wRet<P1,RT>*,TRT (*f)(TP1))
	{
	return CB_FuncTrans1wRet<P1,RT,TRT (*)(TP1)>(f);
	}

template <class P1,class RT,class MemFunc>
class CB_ClassTrans1wRet:public CB_Ftor1wRet<P1,RT>{
	public:
		CB_ClassTrans1wRet(const MemFunc &m):
		  CB_Ftor1wRet<P1,RT>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1)
			  {
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  return CB_BC4_RET_BUG((p1.*memFunc)());
			  }
	};

#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class RT,class TRT,class CallType>
inline CB_ClassTrans1wRet<P1,RT,RebindMemTrans0<TRT,CallType>::MemFunc>
MakeFtor(CB_Ftor1wRet<P1,RT>*,TRT (CallType::* CB_CONST_REF f)())
	{
	typedef TRT (CallType::*MemFunc)();
	return CB_ClassTrans1wRet<P1,RT,MemFunc>(f);
	}
#endif

template <class P1,class RT,class TRT,class CallType>
inline CB_ClassTrans1wRet<P1,RT,RebindMemTrans0c<TRT,CallType>::MemFunc>
MakeFtor(CB_Ftor1wRet<P1,RT>*,TRT (CallType::* CB_CONST_REF f)()const)
	{
	typedef TRT (CallType::*MemFunc)()const;
	return CB_ClassTrans1wRet<P1,RT,MemFunc>(f);
	}


/************************* two args - no return *******************/
template <class P1,class P2>
class CB_Ftor2:public CB_FtorBase{
	public:
		CB_Ftor2(CB_DUMMY_INIT = 0){}
		void operator()(P1 p1,P2 p2)const
			{
			thunk(*this,p1,p2);
			}
		//for STL
		typedef P1 first_argument_type;
		typedef P2 second_argument_type;
		typedef void result_type;
	protected:
		typedef void (*Thunk)(const CB_FtorBase &,P1,P2);
		CB_Ftor2(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		CB_FtorBase(c,f,mf,sz),thunk(t){}
	private:
		Thunk thunk;
	};

template <class P1,class P2,class Callee, class MemFunc>
class CB_MemTrans2:public CB_Ftor2<P1,P2>{
	public:
		CB_MemTrans2(Callee &c,const MemFunc &m):
		  CB_Ftor2<P1,P2>(thunk,&c,0,&m,sizeof(MemFunc)){}
		  static void thunk(const CB_FtorBase &ftor,P1 p1,P2 p2)
			  {
			  Callee *callee = (Callee *)ftor.getCallee();
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  (callee->*memFunc)(p1,p2);
			  }
	};

template <class P1,class P2,class Func>
class CB_FuncTrans2:public CB_Ftor2<P1,P2>{
	public:
		CB_FuncTrans2(Func f):CB_Ftor2<P1,P2>(thunk,0,(PFunc)f,0,0){}
		static void thunk(const CB_FtorBase &ftor,P1 p1,P2 p2)
			{
			(Func(ftor.getFunc()))(p1,p2);
			}
	};


#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class Callee,
class TRT,class CallType,class TP1,class TP2>
inline CB_MemTrans2<P1,P2,Callee,RebindMemTrans2<TRT,CallType,TP1,TP2>::MemFunc>
MakeFtor(CB_Ftor2<P1,P2>*,Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2);
	return CB_MemTrans2<P1,P2,Callee,MemFunc>(c,f);
	}
#endif


template <class P1,class P2,class Callee,
class TRT,class CallType,class TP1,class TP2>
inline CB_MemTrans2<P1,P2,const Callee,
RebindMemTrans2c<TRT,CallType,TP1,TP2>::MemFunc>
MakeFtor(CB_Ftor2<P1,P2>*,const Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2)const;
	return CB_MemTrans2<P1,P2,const Callee,MemFunc>(c,f);
	}

template <class P1,class P2,class TRT,class TP1,class TP2>
inline CB_FuncTrans2<P1,P2,TRT (*)(TP1,TP2)>
MakeFtor(CB_Ftor2<P1,P2>*,TRT (*f)(TP1,TP2))
	{
	return CB_FuncTrans2<P1,P2,TRT (*)(TP1,TP2)>(f);
	}

template <class P1,class P2,class MemFunc>
class CB_ClassTrans2:public CB_Ftor2<P1,P2>{
	public:
		CB_ClassTrans2(const MemFunc &m):
		  CB_Ftor2<P1,P2>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
		  static void thunk(const CB_FtorBase &ftor,P1 p1,P2 p2)
			  {
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  (p1.*memFunc)(p2);
			  }
	};


#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class TRT,class CallType,class TP1>
inline CB_ClassTrans2<P1,P2,
	RebindMemTrans1<TRT,CallType,TP1>::MemFunc>
MakeFtor(CB_Ftor2<P1,P2>*,TRT (CallType::* CB_CONST_REF f)(TP1))
	{
	typedef TRT (CallType::*MemFunc)(TP1);
	return CB_ClassTrans2<P1,P2,MemFunc>(f);
	}
#endif

template <class P1,class P2,class TRT,class CallType,class TP1>
inline CB_ClassTrans2<P1,P2,RebindMemTrans1c<TRT,CallType,TP1>::MemFunc>
MakeFtor(CB_Ftor2<P1,P2>*,TRT (CallType::* CB_CONST_REF f)(TP1)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1)const;
	return CB_ClassTrans2<P1,P2,MemFunc>(f);
	}


/************************* two args - with return *******************/
template <class P1,class P2,class RT>
class CB_Ftor2wRet:public CB_FtorBase{
	public:
		CB_Ftor2wRet(CB_DUMMY_INIT = 0){}
		RT operator()(P1 p1,P2 p2)const
			{
			return thunk(*this,p1,p2);
			}
		//for STL
		typedef P1 first_argument_type;
		typedef P2 second_argument_type;
		typedef RT result_type;
	protected:
		typedef RT (*Thunk)(const CB_FtorBase &,P1,P2);
		CB_Ftor2wRet(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		CB_FtorBase(c,f,mf,sz),thunk(t){}
	private:
		Thunk thunk;
	};

template <class P1,class P2,class RT,class Callee, class MemFunc>
class CB_MemTrans2wRet:public CB_Ftor2wRet<P1,P2,RT>{
	public:
		CB_MemTrans2wRet(Callee &c,const MemFunc &m):
		  CB_Ftor2wRet<P1,P2,RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1,P2 p2)
			  {
			  Callee *callee = (Callee *)ftor.getCallee();
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  return (callee->*memFunc)(p1,p2);
			  }
	};

template <class P1,class P2,class RT,class Func>
class CB_FuncTrans2wRet:public CB_Ftor2wRet<P1,P2,RT>{
	public:
		CB_FuncTrans2wRet(Func f):
		  CB_Ftor2wRet<P1,P2,RT>(thunk,0,(PFunc)f,0,0){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1,P2 p2)
			  {
			  return (Func(ftor.getFunc()))(p1,p2);
			  }
	};


#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class RT,class Callee,
class TRT,class CallType,class TP1,class TP2>
inline CB_MemTrans2wRet<P1,P2,RT,Callee,
RebindMemTrans2<TRT,CallType,TP1,TP2>::MemFunc>
MakeFtor(CB_Ftor2wRet<P1,P2,RT>*,Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2);
	return CB_MemTrans2wRet<P1,P2,RT,Callee,MemFunc>(c,f);
	}
#endif

template <class P1,class P2,class RT,class Callee,
class TRT,class CallType,class TP1,class TP2>
inline CB_MemTrans2wRet<P1,P2,RT,const Callee,
RebindMemTrans2c<TRT,CallType,TP1,TP2>::MemFunc>
MakeFtor(CB_Ftor2wRet<P1,P2,RT>*,const Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2)const;
	return CB_MemTrans2wRet<P1,P2,RT,const Callee,MemFunc>(c,f);
	}

template <class P1,class P2,class RT,class TRT,class TP1,class TP2>
inline CB_FuncTrans2wRet<P1,P2,RT,TRT (*)(TP1,TP2)>
MakeFtor(CB_Ftor2wRet<P1,P2,RT>*,TRT (*f)(TP1,TP2))
	{
	return CB_FuncTrans2wRet<P1,P2,RT,TRT (*)(TP1,TP2)>(f);
	}

template <class P1,class P2,class RT,class MemFunc>
class CB_ClassTrans2wRet:public CB_Ftor2wRet<P1,P2,RT>{
	public:
		CB_ClassTrans2wRet(const MemFunc &m):
		  CB_Ftor2wRet<P1,P2,RT>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1,P2 p2)
			  {
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  return CB_BC4_RET_BUG((p1.*memFunc)(p2));
			  }
	};

#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class RT,class TRT,class CallType,class TP1>
inline CB_ClassTrans2wRet<P1,P2,RT,RebindMemTrans1<TRT,CallType,TP1>::MemFunc>
MakeFtor(CB_Ftor2wRet<P1,P2,RT>*,TRT (CallType::* CB_CONST_REF f)(TP1))
	{
	typedef TRT (CallType::*MemFunc)(TP1);
	return CB_ClassTrans2wRet<P1,P2,RT,MemFunc>(f);
	}
#endif

template <class P1,class P2,class RT,class TRT,class CallType,class TP1>
inline CB_ClassTrans2wRet<P1,P2,RT,RebindMemTrans1c<TRT,CallType,TP1>::MemFunc>
MakeFtor(CB_Ftor2wRet<P1,P2,RT>*,
		  TRT (CallType::* CB_CONST_REF f)(TP1)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1)const;
	return CB_ClassTrans2wRet<P1,P2,RT,MemFunc>(f);
	}


/************************* three args - no return *******************/
template <class P1,class P2,class P3>
class CB_Ftor3:public CB_FtorBase{
	public:
		CB_Ftor3(CB_DUMMY_INIT = 0){}
		void operator()(P1 p1,P2 p2,P3 p3)const
			{
			thunk(*this,p1,p2,p3);
			}
	protected:
		typedef void (*Thunk)(const CB_FtorBase &,P1,P2,P3);
		CB_Ftor3(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		CB_FtorBase(c,f,mf,sz),thunk(t){}
	private:
		Thunk thunk;
	};

template <class P1,class P2,class P3,class Callee, class MemFunc>
class CB_MemTrans3:public CB_Ftor3<P1,P2,P3>{
	public:
		CB_MemTrans3(Callee &c,const MemFunc &m):
		  CB_Ftor3<P1,P2,P3>(thunk,&c,0,&m,sizeof(MemFunc)){}
		  static void thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3)
			  {
			  Callee *callee = (Callee *)ftor.getCallee();
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  (callee->*memFunc)(p1,p2,p3);
			  }
	};

template <class P1,class P2,class P3,class Func>
class CB_FuncTrans3:public CB_Ftor3<P1,P2,P3>{
	public:
		CB_FuncTrans3(Func f):CB_Ftor3<P1,P2,P3>(thunk,0,(PFunc)f,0,0){}
		static void thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3)
			{
			(Func(ftor.getFunc()))(p1,p2,p3);
			}
	};

#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class P3,class Callee,
class TRT,class CallType,class TP1,class TP2,class TP3>
inline CB_MemTrans3<P1,P2,P3,Callee,
RebindMemTrans3<TRT,CallType,TP1,TP2,TP3>::MemFunc>
MakeFtor(CB_Ftor3<P1,P2,P3>*,Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3);
	return CB_MemTrans3<P1,P2,P3,Callee,MemFunc>(c,f);
	}
#endif

template <class P1,class P2,class P3,class Callee,
class TRT,class CallType,class TP1,class TP2,class TP3>
inline CB_MemTrans3<P1,P2,P3,const Callee,
RebindMemTrans3c<TRT,CallType,TP1,TP2,TP3>::MemFunc>
MakeFtor(CB_Ftor3<P1,P2,P3>*,const Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3)const;
	return CB_MemTrans3<P1,P2,P3,const Callee,MemFunc>(c,f);
	}

template <class P1,class P2,class P3,
class TRT,class TP1,class TP2,class TP3>
inline CB_FuncTrans3<P1,P2,P3,TRT (*)(TP1,TP2,TP3)>
MakeFtor(CB_Ftor3<P1,P2,P3>*,TRT (*f)(TP1,TP2,TP3))
	{
	return CB_FuncTrans3<P1,P2,P3,TRT (*)(TP1,TP2,TP3)>(f);
	}

template <class P1,class P2,class P3,class MemFunc>
class CB_ClassTrans3:public CB_Ftor3<P1,P2,P3>{
	public:
		CB_ClassTrans3(const MemFunc &m):
		  CB_Ftor3<P1,P2,P3>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
		  static void thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3)
			  {
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  (p1.*memFunc)(p2,p3);
			  }
	};

#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class P3,class TRT,class CallType,
class TP1,class TP2>
inline CB_ClassTrans3<P1,P2,P3,RebindMemTrans2<TRT,CallType,TP1,TP2>::MemFunc>
MakeFtor(CB_Ftor3<P1,P2,P3>*,TRT (CallType::* CB_CONST_REF f)(TP1,TP2))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2);
	return CB_ClassTrans3<P1,P2,P3,MemFunc>(f);
	}
#endif

template <class P1,class P2,class P3,class TRT,class CallType,
class TP1,class TP2>
inline CB_ClassTrans3<P1,P2,P3,RebindMemTrans2c<TRT,CallType,TP1,TP2>::MemFunc>
MakeFtor(CB_Ftor3<P1,P2,P3>*,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2)const;
	return CB_ClassTrans3<P1,P2,P3,MemFunc>(f);
	}


/************************* three args - with return *******************/
template <class P1,class P2,class P3,class RT>
class CB_Ftor3wRet:public CB_FtorBase{
	public:
		CB_Ftor3wRet(CB_DUMMY_INIT = 0){}
		RT operator()(P1 p1,P2 p2,P3 p3)const
			{
			return (thunk(*this,p1,p2,p3));
			}
	protected:
		typedef RT (*Thunk)(const CB_FtorBase &,P1,P2,P3);
		CB_Ftor3wRet(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		CB_FtorBase(c,f,mf,sz),thunk(t){}
	private:
		Thunk thunk;
	};

template <class P1,class P2,class P3,
class RT,class Callee, class MemFunc>
class CB_MemTrans3wRet:public CB_Ftor3wRet<P1,P2,P3,RT>{
	public:
		CB_MemTrans3wRet(Callee &c,const MemFunc &m):
		  CB_Ftor3wRet<P1,P2,P3,RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3)
			  {
			  Callee *callee = (Callee *)ftor.getCallee();
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
				return ((callee->*memFunc)(p1,p2,p3));
		  }
	};

template <class P1,class P2,class P3,class RT,class Func>
class CB_FuncTrans3wRet:public CB_Ftor3wRet<P1,P2,P3,RT>{
	public:
		CB_FuncTrans3wRet(Func f):
		  CB_Ftor3wRet<P1,P2,P3,RT>(thunk,0,(PFunc)f,0,0){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3)
			  {
			  return (Func(ftor.getFunc()))(p1,p2,p3);
			  }
	};

#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class P3,class RT,class Callee,
class TRT,class CallType,class TP1,class TP2,class TP3>
inline CB_MemTrans3wRet<P1 ,P2,P3,RT,Callee,
RebindMemTrans3<TRT,CallType,TP1,TP2,TP3>::MemFunc>
MakeFtor(CB_Ftor3wRet<P1,P2,P3,RT>*,Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3);
	return CB_MemTrans3wRet<P1,P2,P3,RT,Callee,MemFunc>(c,f);
	}
#endif

template <class P1,class P2,class P3,class RT,class Callee,
class TRT,class CallType,class TP1,class TP2,class TP3>
inline CB_MemTrans3wRet<P1,P2,P3,RT,const Callee,
RebindMemTrans3c<TRT,CallType,TP1,TP2,TP3>::MemFunc>
MakeFtor(CB_Ftor3wRet<P1,P2,P3,RT>*,const Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3)const;
	return CB_MemTrans3wRet<P1,P2,P3,RT,const Callee,MemFunc>(c,f);
	}

template <class P1,class P2,class P3,class RT,
class TRT,class TP1,class TP2,class TP3>
inline CB_FuncTrans3wRet<P1,P2,P3,RT,TRT (*)(TP1,TP2,TP3)>
MakeFtor(CB_Ftor3wRet<P1,P2,P3,RT>*,TRT (*f)(TP1,TP2,TP3))
	{
	return CB_FuncTrans3wRet<P1,P2,P3,RT,TRT (*)(TP1,TP2,TP3)>(f);
	}

template <class P1,class P2,class P3,class RT,class MemFunc>
class CB_ClassTrans3wRet:public CB_Ftor3wRet<P1,P2,P3,RT>{
	public:
		CB_ClassTrans3wRet(const MemFunc &m):
		  CB_Ftor3wRet<P1,P2,P3,RT>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3)
			  {
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  return CB_BC4_RET_BUG((p1.*memFunc)(p2,p3));
			  }
	};

#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class P3,class RT,class TRT,class CallType,
class TP1,class TP2>
inline CB_ClassTrans3wRet<P1,P2,P3,RT,
	RebindMemTrans2<TRT,CallType,TP1,TP2>::MemFunc>
MakeFtor(CB_Ftor3wRet<P1,P2,P3,RT>*,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2);
	return CB_ClassTrans3wRet<P1,P2,P3,RT,MemFunc>(f);
	}
#endif

template <class P1,class P2,class P3,class RT,class TRT,class CallType,
class TP1,class TP2>
inline CB_ClassTrans3wRet<P1,P2,P3,RT,
RebindMemTrans2c<TRT,CallType,TP1,TP2>::MemFunc>
MakeFtor(CB_Ftor3wRet<P1,P2,P3,RT>*,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2)const;
	return CB_ClassTrans3wRet<P1,P2,P3,RT,MemFunc>(f);
	}


/************************* four args - no return *******************/
template <class P1,class P2,class P3,class P4>
class CB_Ftor4:public CB_FtorBase{
	public:
		CB_Ftor4(CB_DUMMY_INIT = 0){}
		void operator()(P1 p1,P2 p2,P3 p3,P4 p4)const
			{
			thunk(*this,p1,p2,p3,p4);
			}
	protected:
		typedef void (*Thunk)(const CB_FtorBase &,P1,P2,P3,P4);
		CB_Ftor4(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		CB_FtorBase(c,f,mf,sz),thunk(t){}
	private:
		Thunk thunk;
	};

template <class P1,class P2,class P3,class P4,
class Callee, class MemFunc>
class CB_MemTrans4:public CB_Ftor4<P1,P2,P3,P4>{
	public:
		CB_MemTrans4(Callee &c,const MemFunc &m):
		  CB_Ftor4<P1,P2,P3,P4>(thunk,&c,0,&m,sizeof(MemFunc)){}
		  static void thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3,P4 p4)
			  {
			  Callee *callee = (Callee *)ftor.getCallee();
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  (callee->*memFunc)(p1,p2,p3,p4);
			  }
	};

template <class P1,class P2,class P3,class P4,class Func>
class CB_FuncTrans4:public CB_Ftor4<P1,P2,P3,P4>{
	public:
		CB_FuncTrans4(Func f):
		  CB_Ftor4<P1,P2,P3,P4>(thunk,0,(PFunc)f,0,0){}
		  static void thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3,P4 p4)
			  {
			  (Func(ftor.getFunc()))(p1,p2,p3,p4);
			  }
	};

#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class P3,class P4,class Callee,
class TRT,class CallType,class TP1,class TP2,class TP3,class TP4>
inline CB_MemTrans4<P1,P2,P3,P4,Callee,
RebindMemTrans4<TRT,CallType,TP1,TP2,TP3,TP4>::MemFunc>
MakeFtor(CB_Ftor4<P1,P2,P3,P4>*,Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3,TP4))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4);
	return CB_MemTrans4<P1,P2,P3,P4,Callee,MemFunc>(c,f);
	}
#endif

template <class P1,class P2,class P3,class P4,class Callee,
class TRT,class CallType,class TP1,class TP2,class TP3,class TP4>
inline CB_MemTrans4<P1,P2,P3,P4,const Callee,
RebindMemTrans4c<TRT,CallType,TP1,TP2,TP3,TP4>::MemFunc>
MakeFtor(CB_Ftor4<P1,P2,P3,P4>*,const Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3,TP4)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4)const;
	return CB_MemTrans4<P1,P2,P3,P4,const Callee,MemFunc>(c,f);
	}

template <class P1,class P2,class P3,class P4,
class TRT,class TP1,class TP2,class TP3,class TP4>
inline CB_FuncTrans4<P1,P2,P3,P4,TRT (*)(TP1,TP2,TP3,TP4)>
MakeFtor(CB_Ftor4<P1,P2,P3,P4>*,TRT (*f)(TP1,TP2,TP3,TP4))
	{
	return CB_FuncTrans4<P1,P2,P3,P4,TRT (*)(TP1,TP2,TP3,TP4)>(f);
	}

template <class P1,class P2,class P3,class P4,class MemFunc>
class CB_ClassTrans4:public CB_Ftor4<P1,P2,P3,P4>{
	public:
		CB_ClassTrans4(const MemFunc &m):
		  CB_Ftor4<P1,P2,P3,P4>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
		  static void thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3,P4 p4)
			  {
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
			  (p1.*memFunc)(p2,p3,p4);
			  }
	};

#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class P3,class P4,class TRT,class CallType,
class TP1,class TP2,class TP3>
inline CB_ClassTrans4<P1,P2,P3,P4,
RebindMemTrans3<TRT,CallType,TP1,TP2,TP3>::MemFunc>
MakeFtor(CB_Ftor4<P1,P2,P3,P4>*,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3);
	return CB_ClassTrans4<P1,P2,P3,P4,MemFunc>(f);
	}
#endif

template <class P1,class P2,class P3,class P4,class TRT,class CallType,
class TP1,class TP2,class TP3>
inline CB_ClassTrans4<P1,P2,P3,P4,
RebindMemTrans3c<TRT,CallType,TP1,TP2,TP3>::MemFunc>
MakeFtor(CB_Ftor4<P1,P2,P3,P4>*,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3)const;
	return CB_ClassTrans4<P1,P2,P3,P4,MemFunc>(f);
	}


/************************* four args - with return *******************/
template <class P1,class P2,class P3,class P4,class RT>
class CB_Ftor4wRet:public CB_FtorBase{
	public:
		CB_Ftor4wRet(CB_DUMMY_INIT = 0){}
		RT operator()(P1 p1,P2 p2,P3 p3,P4 p4)const
			{
			return (thunk(*this,p1,p2,p3,p4));	
			}
	protected:
		typedef RT (*Thunk)(const CB_FtorBase &,P1,P2,P3,P4);
		CB_Ftor4wRet(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		CB_FtorBase(c,f,mf,sz),thunk(t){}
	private:
		Thunk thunk;
	};

template <class P1,class P2,class P3,class P4,class RT,
class Callee, class MemFunc>
class CB_MemTrans4wRet:public CB_Ftor4wRet<P1,P2,P3,P4,RT>{
	public:
		CB_MemTrans4wRet(Callee &c,const MemFunc &m):
		  CB_Ftor4wRet<P1,P2,P3,P4,RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3,P4 p4)
			  {
			  Callee *callee = (Callee *)ftor.getCallee();
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
				return ((callee->*memFunc)(p1,p2,p3,p4));	  
		  }
	};

template <class P1,class P2,class P3,class P4,class RT,class Func>
class CB_FuncTrans4wRet:public CB_Ftor4wRet<P1,P2,P3,P4,RT>{
	public:
		CB_FuncTrans4wRet(Func f):
		  CB_Ftor4wRet<P1,P2,P3,P4,RT>(thunk,0,(PFunc)f,0,0){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3,P4 p4)
			  {
			  return (Func(ftor.getFunc()))(p1,p2,p3,p4);
			  }
	};

#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class P3,class P4,class RT,class Callee,
class TRT,class CallType,class TP1,class TP2,class TP3,class TP4>
inline CB_MemTrans4wRet<P1,P2,P3,P4,RT,Callee,
RebindMemTrans4<TRT,CallType,TP1,TP2,TP3,TP4>::MemFunc>
MakeFtor(CB_Ftor4wRet<P1,P2,P3,P4,RT>*,Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3,TP4))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4);
	return CB_MemTrans4wRet<P1,P2,P3,P4,RT,Callee,MemFunc>(c,f);
	}
#endif

template <class P1,class P2,class P3,class P4,class RT,class Callee,
class TRT,class CallType,class TP1,class TP2,class TP3,class TP4>
inline CB_MemTrans4wRet<P1,P2,P3,P4,RT,const Callee,
RebindMemTrans4c<TRT,CallType,TP1,TP2,TP3,TP4>::MemFunc>
MakeFtor(CB_Ftor4wRet<P1,P2,P3,P4,RT>*,const Callee &c,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3,TP4)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3,TP4)const;
	return CB_MemTrans4wRet<P1,P2,P3,P4,RT,const Callee,MemFunc>(c,f);
	}

template <class P1,class P2,class P3,class P4,class RT,
class TRT,class TP1,class TP2,class TP3,class TP4>
inline CB_FuncTrans4wRet<P1,P2,P3,P4,RT,TRT (*)(TP1,TP2,TP3,TP4)>
MakeFtor(CB_Ftor4wRet<P1,P2,P3,P4,RT>*,TRT (*f)(TP1,TP2,TP3,TP4))
	{
	return CB_FuncTrans4wRet
		<P1,P2,P3,P4,RT,TRT (*)(TP1,TP2,TP3,TP4)>(f);
	}


template <class P1,class P2,class P3,class P4,class RT,class MemFunc>
class CB_ClassTrans4wRet:public CB_Ftor4wRet<P1,P2,P3,P4,RT>{
	public:
		CB_ClassTrans4wRet(const MemFunc &m):
		  CB_Ftor4wRet<P1,P2,P3,P4,RT>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
		  static RT thunk(const CB_FtorBase &ftor,P1 p1,P2 p2,P3 p3,P4 p4)
			  {
			  MemFunc &memFunc CB_CTOR_STYLE_INIT
				  (*(MemFunc*)(void *)(ftor.getMemFunc()));
				return ((p1.*memFunc)(p2,p3,p4));
		  }
	};

#if !defined(CB_CANT_OVERLOAD_ON_CONSTNESS)
template <class P1,class P2,class P3,class P4,class RT,class TRT,
class CallType,class TP1,class TP2,class TP3>
inline CB_ClassTrans4wRet<P1,P2,P3,P4,RT,
RebindMemTrans3<TRT,CallType,TP1,TP2,TP3>::MemFunc>
MakeFtor(CB_Ftor4wRet<P1,P2,P3,P4,RT>*,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3);
	return CB_ClassTrans4wRet<P1,P2,P3,P4,RT,MemFunc>(f);
	}
#endif

template <class P1,class P2,class P3,class P4,class RT,class TRT,
class CallType,class TP1,class TP2,class TP3>
inline CB_ClassTrans4wRet<P1,P2,P3,P4,RT,
RebindMemTrans3c<TRT,CallType,TP1,TP2,TP3>::MemFunc>
MakeFtor(CB_Ftor4wRet<P1,P2,P3,P4,RT>*,
		  TRT (CallType::* CB_CONST_REF f)(TP1,TP2,TP3)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3)const;
	return CB_ClassTrans4wRet<P1,P2,P3,P4,RT,MemFunc>(f);
	}



#endif //CALLBACK_LIB_HICKEY



