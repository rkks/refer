#include <iostream.h>
#include "callback.hpp"

// Illustrates 'Member of First Argument' variation

// If you haven't tried this yet, check it out

class X{
public:
	X(int n):i(n){}
	void foo() const
		{
		cout << "X::foo " <<i<<endl;
		}
	int i;
};

void bar(const CBFunctor1<const X &> func)
	{
	X xa[4] = {1,2,3,4};
	for(int i=0;i<4;i++)
		func(xa[i]);
	}

void fred(const X &x)
	{
	cout << "fred "<<(x.i * 10) << endl;
	}

void main()
	{
	//bar, through a single interface, can call a member function of X
	//or a function taking X
	bar(makeFunctor((CBFunctor1<const X &> *)0,&X::foo));
	bar(makeFunctor((CBFunctor1<const X &> *)0,&fred));
	}

