// Six compiler bugs that prevented a popular compiler
// from compiling the callback library as originally written

//If your compiler has similar problems, try the RHCB flags mentioned

//////////////////////////////////////////////////////
#if defined(BUG1)
//unable to call operator() on template class reference
//Library user must work around by passing by value

template <class T>
class Functor{
public:
	void operator()(T t)const{};
};

void foo(const Functor<int> &f)
	{
	f(1);	
	//works when f is passed by value
	}

#endif

//////////////////////////////////////////////////////
#if defined(BUG2)
//Does not accept parenthesized initialization of references
//worked around with RHCB_CANT_INIT_REFERENCE_CTOR_STYLE	

void foo()
	{
	int i;
	int &ir(i);
	}

#endif

//////////////////////////////////////////////////////
#if defined(BUG3)
//cannot overload template functions on const'ness of pointer-to-member-func
//worked around with RHCB_CANT_OVERLOAD_ON_CONSTNESS		

class X{
public:
	void foo();
	void bar()const;
};

template <class T>
void f(void (T::*func)())
	{
	}

template <class T>
void f(void (T::*func)()const)
	{
	}

void main()
	{
	f(&X::bar);
	f(&X::foo);
	//works if f is not a template function
	}

#endif

//////////////////////////////////////////////////////
#if defined(BUG4)
//chokes on reference to pointer to member function in template function args
//worked around with RHCB_CANT_PASS_MEMFUNC_BY_REFERENCE

class X{
public:
	void foo();
	void bar()const;
};

template <class T>
void f(void (T::* const &func)())
	{
	}

template <class T>
void bar(const T &)
	{
	}

void main()
	{
	bar(5);	//obviously does trivial conversion X => const X &
			//on template function args yet:

	f(&X::foo);

	//works if f is not a template function
	}

#endif

//////////////////////////////////////////////////////
#if defined(BUG5)
//incorrectly allows binding of pointer-to-non-const member func
//to pointer-to-const-member-func in template function args

//Library user must work around by manually ensuring the
//member function is const

class X{
public:
	void foo();
	void bar()const;
};


template <class T>
void f(void (T::* func)()const, const T &t)
	{
	(t.*func)();
	}

void main()
	{
	X x;
	f(&X::foo, x);	//this call should fail but does not
	
	//does correctly detect error when f is not a template function
	}

#endif

//////////////////////////////////////////////////////
#if defined(BUG6)
//fails to apply pointer conversion 0=>pointer-type in
//all copy-initialization contexts:

//worked around with RHCB_WONT_PERFORM_PTR_CONVERSION

class Y;

class X{
public:
	X(Y *);
};

void bar(X);

void foo()
	{
	X x = 0;  //ok, yet:
	
	bar(0);

	x = 0;
	}

#endif

