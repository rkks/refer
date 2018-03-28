// This example shows the use of the comparison
// ops to enable removal from containers of functors

// Note: you must compile and link in callback.cpp to get the
// comparison ops required by less<>

// Note: this example requires STL

#include "callback.hpp"
#include <set.h>
#include <iostream.h>

class Notifier{
public:
	typedef CBFunctor1<int> CBType;
	typedef set<CBType,less<CBType> > ClientList;

	static CBType *cbType(){return 0;}	//to ease use of makeFunctor

	void notifyAll(int i) const
		{
		for(ClientList::iterator iter = clients.begin();
				iter != clients.end();++iter)
			{
			(*iter)(i);
			}
		}

	void addClient(const CBType &client)
		{
		clients.insert(client);
		}

	void removeClient(const CBType &client)
		{
		clients.erase(client);
		}

private:
	ClientList clients;
	};

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

class X{
public:
	X(int i):num(i){}
	void foo(int i)
		{cout << "X #" <<num << " foo("<<i<<")"<<endl;}
	void bar(int i)
			{cout << "X #" <<num << " bar("<<i<<")"<<endl;}
private:
	int num;
};

main()
	{
	Notifier n;

	B aB;
	D aD;
	X x1(1),x2(2),x3(3);

	Notifier::CBType nullFunctor, cb1, cb2;

	if(cb1 == cb2)
		{cout << "null functors compare equal" << endl;}

   cb1 = makeFunctor(n.cbType(),x1,&X::foo);
   cb2 = makeFunctor(n.cbType(),x1,&X::foo);

	if(cb1 == cb2)
		{cout << "aliased functors compare equal" << endl;}

	cb2 = makeFunctor(n.cbType(),&fred);
	if(cb1 != nullFunctor && cb2 != nullFunctor)
		{cout << "null functors don't match set functors" << endl;}
	
	n.addClient(makeFunctor(n.cbType(),&fred));
	n.addClient(makeFunctor(n.cbType(),aB,&B::ricky));
	n.addClient(makeFunctor(n.cbType(),x1,&X::bar));
	n.addClient(makeFunctor(n.cbType(),x3,&X::foo));
	n.addClient(makeFunctor(n.cbType(),aD,&B::ricky));
	n.addClient(makeFunctor(n.cbType(),&ethel));
	n.addClient(makeFunctor(n.cbType(),x2,&X::foo));
	n.addClient(makeFunctor(n.cbType(),x1,&X::foo));

	n.notifyAll(1);

	cout << "\nRemoving ethel, B, and x2 \n\n";
	n.removeClient(makeFunctor(n.cbType(),&ethel));
	n.removeClient(makeFunctor(n.cbType(),aB,&B::ricky));
	n.removeClient(makeFunctor(n.cbType(),x2,&X::foo));

	n.notifyAll(2);
	}

