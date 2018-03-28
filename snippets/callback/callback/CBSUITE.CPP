//************************** cbsuite.cpp ***************
// test suite for callbacks
// Copyright 1994 Rich Hickey
/* Permission to use, copy, modify, distribute and sell this software
 * for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Rich Hickey makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
*/

// 2/14/94 Rich Hickey
#include <iostream.h>
#include "callback.hpp"

class Map{
public:
	Map(int i):data(i){}
	operator int()const{return data;}
private:
	int data;
};

void func0()
	{cout<<" ::func0:\t" << endl;}
void func1(int i)
	{cout<<" ::func1:\t" << i << endl;}
void func2(int i,long l)
	{cout<<" ::func2:\t" << i <<' '<< l << endl;}
void func3(int i,long l,float f)
	{cout<<" ::func3:\t"<<i<<' '<<l<<' '<<f<<endl;}
void func4(int i,long l,float f,double d)
	{cout<<" ::func4:\t"<<i<<' '<<l<<' '<<f<<' '<<d <<endl;}
int func0wRet()
	{cout<<" ::func0wRet:\t" << endl;return 0;}
int func1wRet(int i)
	{cout<<" ::func1wRet:\t" << i << endl;return i;}
int func2wRet(int i,long l)
	{cout<<" ::func2wRet:\t" << i <<' '<< l << endl;return i;}
int func3wRet(int i,long l,float f)
	{cout<<" ::func3wRet:\t"<<i<<' '<<l<<' '<<f<<endl;return i;}
int func4wRet(int i,long l,float f,double d)
	{cout<<" ::func4wRet:\t"<<i<<' '<<l<<' '<<f<<' '<<d <<endl;return i;}

class X{
public:
virtual void func0()
	{cout<<"X::func0:\t" << endl;}
virtual void func1(int i)
	{cout<<"X::func1:\t" << i << endl;}
virtual void func2(int i,long l)
	{cout<<"X::func2:\t" << i <<' '<< l << endl;}
virtual void func3(int i,long l,float f)
	{cout<<"X::func3:\t"<<i<<' '<<l<<' '<<f<<endl;}
virtual void func4(int i,long l,float f,double d)
	{cout<<"X::func4:\t"<<i<<' '<<l<<' '<<f<<' '<<d <<endl;}
virtual int func0wRet()
	{cout<<"X::func0wRet:\t" << endl;return 0;}
virtual int func1wRet(int i)
	{cout<<"X::func1wRet:\t" << i << endl;return i;}
virtual int func2wRet(int i,long l)
	{cout<<"X::func2wRet:\t" << i <<' '<< l << endl;return i;}
virtual int func3wRet(int i,long l,float f)
	{cout<<"X::func3wRet:\t"<<i<<' '<<l<<' '<<f<<endl;return i;}
virtual int func4wRet(int i,long l,float f,double d)
	{cout<<"X::func4wRet:\t"<<i<<' '<<l<<' '<<f<<' '<<d <<endl;return i;}

virtual void func0c()const
	{cout<<"X::func0c:\t" << endl;}
virtual void func1c(int i)const
	{cout<<"X::func1c:\t" << i << endl;}
virtual void func2c(int i,long l)const
	{cout<<"X::func2c:\t" << i <<' '<< l << endl;}
virtual void func3c(int i,long l,float f)const
	{cout<<"X::func3c:\t"<<i<<' '<<l<<' '<<f<<endl;}
virtual void func4c(int i,long l,float f,double d)const
	{cout<<"X::func4c:\t"<<i<<' '<<l<<' '<<f<<' '<<d <<endl;}
virtual int func0wRetc()const
	{cout<<"X::func0wRetc:\t" << endl;return 0;}
virtual int func1wRetc(int i)const
	{cout<<"X::func1wRetc:\t" << i << endl;return i;}
virtual int func2wRetc(int i,long l)const
	{cout<<"X::func2wRetc:\t" << i <<' '<< l << endl;return i;}
virtual int func3wRetc(int i,long l,float f)const
	{cout<<"X::func3wRetc:\t"<<i<<' '<<l<<' '<<f<<endl;return i;}
virtual int func4wRetc(int i,long l,float f,double d)const
	{cout<<"X::func4wRetc:\t"<<i<<' '<<l<<' '<<f<<' '<<d <<endl;return i;}
};

class Y:public X{
public:
virtual void func0()
	{cout<<"Y::func0:\t" << endl;}
virtual void func1(int i)
	{cout<<"Y::func1:\t" << i << endl;}
virtual void func2(int i,long l)
	{cout<<"Y::func2:\t" << i <<' '<< l << endl;}
virtual void func3(int i,long l,float f)
	{cout<<"Y::func3:\t"<<i<<' '<<l<<' '<<f<<endl;}
virtual void func4(int i,long l,float f,double d)
	{cout<<"Y::func4:\t"<<i<<' '<<l<<' '<<f<<' '<<d <<endl;}
virtual int func0wRet()
	{cout<<"Y::func0wRet:\t" << endl;return 0;}
virtual int func1wRet(int i)
	{cout<<"Y::func1wRet:\t" << i << endl;return i;}
virtual int func2wRet(int i,long l)
	{cout<<"Y::func2wRet:\t" << i <<' '<< l << endl;return i;}
virtual int func3wRet(int i,long l,float f)
	{cout<<"Y::func3wRet:\t"<<i<<' '<<l<<' '<<f<<endl;return i;}
virtual int func4wRet(int i,long l,float f,double d)
	{cout<<"Y::func4wRet:\t"<<i<<' '<<l<<' '<<f<<' '<<d <<endl;return i;}

virtual void func0c()const
	{cout<<"Y::func0c:\t" << endl;}
virtual void func1c(int i)const
	{cout<<"Y::func1c:\t" << i << endl;}
virtual void func2c(int i,long l)const
	{cout<<"Y::func2c:\t" << i <<' '<< l << endl;}
virtual void func3c(int i,long l,float f)const
	{cout<<"Y::func3c:\t"<<i<<' '<<l<<' '<<f<<endl;}
virtual void func4c(int i,long l,float f,double d)const
	{cout<<"Y::func4c:\t"<<i<<' '<<l<<' '<<f<<' '<<d <<endl;}
virtual int func0wRetc()const
	{cout<<"Y::func0wRetc:\t" << endl;return 0;}
virtual int func1wRetc(int i)const
	{cout<<"Y::func1wRetc:\t" << i << endl;return i;}
virtual int func2wRetc(int i,long l)const
	{cout<<"Y::func2wRetc:\t" << i <<' '<< l << endl;return i;}
virtual int func3wRetc(int i,long l,float f)const
	{cout<<"Y::func3wRetc:\t"<<i<<' '<<l<<' '<<f<<endl;return i;}
virtual int func4wRetc(int i,long l,float f,double d)const
	{cout<<"Y::func4wRetc:\t"<<i<<' '<<l<<' '<<f<<' '<<d <<endl;return i;}
};


void main()
	{
	X x;
	Y y;
	int i=1,j=1,k=1,l=1;
	CBFunctor0 f0;
	if(f0)
		cout << "Bad initialization"<<endl;
	f0 = 0;
	f0 = makeFunctor((CBFunctor0 *)0,func0);
	f0();
	f0 = makeFunctor((CBFunctor0 *)0,x,&X::func0);
	f0();
	f0 = makeFunctor((CBFunctor0 *)0,y,&X::func0);
	f0();
#ifndef __WATCOMC__
	f0 = makeFunctor((CBFunctor0 *)0,y,&Y::func0c);
	f0();
#endif
	CBFunctor1<int> f1;
	if(f1)
		cout << "Bad initialization"<<endl;
	f1=0;
	f1 = makeFunctor((CBFunctor1<int> *)0,func1);
	f1(i++);
	f1 = makeFunctor((CBFunctor1<int> *)0,x,&X::func1);
	f1(i++);
	f1 = makeFunctor((CBFunctor1<int> *)0,y,&X::func1);
	f1(i++);
	f1 = makeFunctor((CBFunctor1<int> *)0,y,&Y::func1c);
	f1(i++);
	CBFunctor1<Map> fm1;
	if(fm1)
		cout << "Bad initialization"<<endl;
	fm1=0;
	fm1 = makeFunctor((CBFunctor1<Map> *)0,func1);
	fm1(i++);
	fm1 = makeFunctor((CBFunctor1<Map> *)0,x,&X::func1);
	fm1(i++);
	fm1 = makeFunctor((CBFunctor1<Map> *)0,y,&X::func1);
	fm1(i++);
	fm1 = makeFunctor((CBFunctor1<Map> *)0,y,&Y::func1c);
	fm1(i++);

	CBFunctor2<int,long> f2;
	if(f2)
		cout << "Bad initialization"<<endl;
   f2 = 0;
	f2 = makeFunctor((CBFunctor2<int,long> *)0,func2);
	f2(i++,j++);
	f2 = makeFunctor((CBFunctor2<int,long> *)0,x,&X::func2);
	f2(i++,j++);
	f2 = makeFunctor((CBFunctor2<int,long> *)0,y,&X::func2);
	f2(i++,j++);
	f2 = makeFunctor((CBFunctor2<int,long> *)0,y,&Y::func2c);
	f2(i++,j++);
	CBFunctor2<Map,long> fm2;
	if(fm2)
		cout << "Bad initialization"<<endl;
   fm2 = 0;
	fm2 = makeFunctor((CBFunctor2<Map,long> *)0,func2);
	fm2(i++,j++);
	fm2 = makeFunctor((CBFunctor2<Map,long> *)0,x,&X::func2);
	fm2(i++,j++);
	fm2 = makeFunctor((CBFunctor2<Map,long> *)0,y,&X::func2);
	fm2(i++,j++);
	fm2 = makeFunctor((CBFunctor2<Map,long> *)0,y,&Y::func2c);
	fm2(i++,j++);

	CBFunctor3<int,long,float> f3;
	if(f3)
		cout << "Bad initialization"<<endl;
   f3 = 0;
	f3 = makeFunctor((CBFunctor3<int,long,float> *)0,func3);
	f3(i++,j++,k++);
	f3 = makeFunctor((CBFunctor3<int,long,float> *)0,x,&X::func3);
	f3(i++,j++,k++);
	f3 = makeFunctor((CBFunctor3<int,long,float> *)0,y,&X::func3);
	f3(i++,j++,k++);
	f3 = makeFunctor((CBFunctor3<int,long,float> *)0,y,&Y::func3c);
	f3(i++,j++,k++);
	CBFunctor3<Map,int,long> fm3;
	if(fm3)
		cout << "Bad initialization"<<endl;
   fm3 = 0;
	fm3 = makeFunctor((CBFunctor3<Map,int,long> *)0,func3);
	fm3(i++,j++,k++);
	fm3 = makeFunctor((CBFunctor3<Map,int,long> *)0,x,&X::func3);
	fm3(i++,j++,k++);
	fm3 = makeFunctor((CBFunctor3<Map,int,long> *)0,y,&X::func3);
	fm3(i++,j++,k++);
	fm3 = makeFunctor((CBFunctor3<Map,int,long> *)0,y,&Y::func3c);
	fm3(i++,j++,k++);

	CBFunctor4<int,long,float,double> f4;
	if(f4)
		cout << "Bad initialization"<<endl;
   f4 = 0;
	f4 = makeFunctor((CBFunctor4<int,long,float,double> *)0,func4);
	f4(i++,j++,k++,l++);
	f4 = makeFunctor((CBFunctor4<int,long,float,double> *)0,x,&X::func4);
	f4(i++,j++,k++,l++);
	f4 = makeFunctor((CBFunctor4<int,long,float,double> *)0,y,&X::func4);
	f4(i++,j++,k++,l++);
	f4 = makeFunctor((CBFunctor4<int,long,float,double> *)0,y,&Y::func4c);
	f4(i++,j++,k++,l++);
	CBFunctor4<Map,int,long,float> fm4;
	if(fm4)
		cout << "Bad initialization"<<endl;
   fm4 = 0;
	fm4 = makeFunctor((CBFunctor4<Map,int,long,float> *)0,func4);
	fm4(i++,j++,k++,l++);
	fm4 = makeFunctor((CBFunctor4<Map,int,long,float> *)0,x,&X::func4);
	fm4(i++,j++,k++,l++);
	fm4 = makeFunctor((CBFunctor4<Map,int,long,float> *)0,y,&X::func4);
	fm4(i++,j++,k++,l++);
	fm4 = makeFunctor((CBFunctor4<Map,int,long,float> *)0,y,&Y::func4c);
	fm4(i++,j++,k++,l++);

////////////////////////////////////////////////////////////////
	CBFunctor0wRet<int> f0r;
	if(f0r)
		cout << "Bad initialization"<<endl;
   f0r = 0;
	f0r = makeFunctor((CBFunctor0wRet<int> *)0,func0wRet);
	cout << f0r()<<endl;
	f0r = makeFunctor((CBFunctor0wRet<int> *)0,x,&X::func0wRet);
	cout << f0r()<<endl;
	f0r = makeFunctor((CBFunctor0wRet<int> *)0,y,&X::func0wRet);
	cout << f0r()<<endl;
	f0r = makeFunctor((CBFunctor0wRet<int> *)0,y,&Y::func0wRetc);
	cout << f0r()<<endl;
	CBFunctor1wRet<int,int> f1r;
	if(f1r)
		cout << "Bad initialization"<<endl;
   f1r = 0;
	f1r = makeFunctor((CBFunctor1wRet<int,int> *)0,func1wRet);
	cout << f1r(i++)<<endl;
	f1r = makeFunctor((CBFunctor1wRet<int,int> *)0,x,&X::func1wRet);
	cout << f1r(i++)<<endl;
	f1r = makeFunctor((CBFunctor1wRet<int,int> *)0,y,&X::func1wRet);
	cout << f1r(i++)<<endl;
	f1r = makeFunctor((CBFunctor1wRet<int,int> *)0,y,&Y::func1wRetc);
	cout << f1r(i++)<<endl;
	CBFunctor1wRet<Map,int> fm1r;
	if(fm1r)
		cout << "Bad initialization"<<endl;
   fm1r = 0;
	fm1r = makeFunctor((CBFunctor1wRet<Map,int> *)0,func1wRet);
	cout << fm1r(i++)<<endl;
	fm1r = makeFunctor((CBFunctor1wRet<Map,int> *)0,x,&X::func1wRet);
	cout << fm1r(i++)<<endl;
	fm1r = makeFunctor((CBFunctor1wRet<Map,int> *)0,y,&X::func1wRet);
	cout << fm1r(i++)<<endl;
	fm1r = makeFunctor((CBFunctor1wRet<Map,int> *)0,y,&Y::func1wRetc);
	cout << fm1r(i++)<<endl;

	CBFunctor2wRet<int,long,int> f2r;
	if(f2r)
		cout << "Bad initialization"<<endl;
   f2r = 0;
	f2r = makeFunctor((CBFunctor2wRet<int,long,int> *)0,func2wRet);
	cout << f2r(i++,j++)<<endl;
	f2r = makeFunctor((CBFunctor2wRet<int,long,int> *)0,x,&X::func2wRet);
	cout << f2r(i++,j++)<<endl;
	f2r = makeFunctor((CBFunctor2wRet<int,long,int> *)0,y,&X::func2wRet);
	cout << f2r(i++,j++)<<endl;
	f2r = makeFunctor((CBFunctor2wRet<int,long,int> *)0,y,&Y::func2wRetc);
	cout << f2r(i++,j++)<<endl;
	CBFunctor2wRet<Map,long,int> fm2r;
	fm2r = makeFunctor((CBFunctor2wRet<Map,long,int> *)0,func2wRet);
	cout << fm2r(i++,j++)<<endl;
	fm2r = makeFunctor((CBFunctor2wRet<Map,long,int> *)0,x,&X::func2wRet);
	cout << fm2r(i++,j++)<<endl;
	fm2r = makeFunctor((CBFunctor2wRet<Map,long,int> *)0,y,&X::func2wRet);
	cout << fm2r(i++,j++)<<endl;
	fm2r = makeFunctor((CBFunctor2wRet<Map,long,int> *)0,y,&Y::func2wRetc);
	cout << fm2r(i++,j++)<<endl;

	CBFunctor3wRet<int,long,float,int> f3r;
	if(f3r)
		cout << "Bad initialization"<<endl;
   f3r = 0;
	f3r = makeFunctor((CBFunctor3wRet<int,long,float,int> *)0,func3wRet);
	cout << f3r(i++,j++,k++)<<endl;
	f3r = makeFunctor((CBFunctor3wRet<int,long,float,int> *)0,x,&X::func3wRet);
	cout << f3r(i++,j++,k++)<<endl;
	f3r = makeFunctor((CBFunctor3wRet<int,long,float,int> *)0,y,&X::func3wRet);
	cout << f3r(i++,j++,k++)<<endl;
	f3r = makeFunctor((CBFunctor3wRet<int,long,float,int> *)0,y,&Y::func3wRetc);
	cout << f3r(i++,j++,k++)<<endl;
	CBFunctor3wRet<Map,long,float,int> fm3r;
	if(fm3r)
		cout << "Bad initialization"<<endl;
   fm3r = 0;
	fm3r = makeFunctor((CBFunctor3wRet<Map,long,float,int> *)0,func3wRet);
	cout << fm3r(i++,j++,k++)<<endl;
	fm3r = makeFunctor((CBFunctor3wRet<Map,long,float,int> *)0,x,&X::func3wRet);
	cout << fm3r(i++,j++,k++)<<endl;
	fm3r = makeFunctor((CBFunctor3wRet<Map,long,float,int> *)0,y,&X::func3wRet);
	cout << fm3r(i++,j++,k++)<<endl;
	fm3r = makeFunctor((CBFunctor3wRet<Map,long,float,int> *)0,y,&Y::func3wRetc);
	cout << fm3r(i++,j++,k++)<<endl;

	CBFunctor4wRet<int,long,float,double,int> f4r;
	if(f4r)
		cout << "Bad initialization"<<endl;
   f4r = 0;
	f4r = makeFunctor((CBFunctor4wRet<int,long,float,double,int> *)0,func4wRet);
	cout << f4r(i++,j++,k++,l++)<<endl;
	f4r = makeFunctor((CBFunctor4wRet<int,long,float,double,int> *)0,x,&X::func4wRet);
	cout << f4r(i++,j++,k++,l++)<<endl;
	f4r = makeFunctor((CBFunctor4wRet<int,long,float,double,int> *)0,y,&X::func4wRet);
	cout << f4r(i++,j++,k++,l++)<<endl;
	f4r = makeFunctor((CBFunctor4wRet<int,long,float,double,int> *)0,y,&Y::func4wRetc);
	cout << f4r(i++,j++,k++,l++)<<endl;
	CBFunctor4wRet<Map,int,long,float,Map> fm4r;
	if(fm4r)
		cout << "Bad initialization"<<endl;
   fm4r = 0;
	fm4r = makeFunctor((CBFunctor4wRet<Map,int,long,float,Map> *)0,func4wRet);
	cout << fm4r(i++,j++,k++,l++)<<endl;
	fm4r = makeFunctor((CBFunctor4wRet<Map,int,long,float,Map> *)0,x,&X::func4wRet);
	cout << fm4r(i++,j++,k++,l++)<<endl;
	fm4r = makeFunctor((CBFunctor4wRet<Map,int,long,float,Map> *)0,y,&X::func4wRet);
	cout << fm4r(i++,j++,k++,l++)<<endl;
	fm4r = makeFunctor((CBFunctor4wRet<Map,int,long,float,Map> *)0,y,&Y::func4wRetc);
	cout << fm4r(i++,j++,k++,l++)<<endl;
	}
