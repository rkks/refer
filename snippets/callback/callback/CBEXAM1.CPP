#include <iostream.h>
#include "callback.hpp"


//do5Times() is a function that takes a functor and invokes it 5 times

#if defined(_MSC_VER)	//MS can't deal with op() call on reference	to template
void do5Times(CBFunctor1<int> doIt)
#else
void do5Times(const CBFunctor1<int> &doIt)
#endif
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
	virtual void ricky(int i)  const
	   {cout << "B::ricky: " << i<<endl;}
};

class D:public B{
public:
	void ricky(int i)	 const
	   {cout << "D::ricky: " << i<<endl;}
};

void main()
	{
	//create a typedef of the functor type to simplify dummy argument
	CBFunctor1<int> *FtorType = 0;

	CBFunctor1<int> ftor;	//a functor variable
	//make functor from ptr-to-function
	ftor = makeFunctor(FtorType,fred);
	do5Times(ftor);
	//note ethel is not an exact match
	ftor = makeFunctor(FtorType,ethel);
	do5Times(ftor);

	//create a D object to be a callback target
	D myD;
	//make functor from object and ptr-to-member-func
	ftor = makeFunctor(FtorType,myD,&B::ricky);
	do5Times(ftor);
	}

