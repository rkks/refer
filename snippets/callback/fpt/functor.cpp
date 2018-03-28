// This code was compiled and tested with Borland C++ Builder 5.0.
#pragma hdrstop       // Borland C++ Builder specific
#pragma argsused      // do.

//----------------------------------------------------------------------------//
//    Copyright (c) 2000-2005 by Lars Haendel                                 //
//    Home: http://www.newty.de                                               //
//    Last modified: 06 january 2005                                          //
//                                                                            //
//    Example code for 'The Function Pointer Tutorials' chapter 4 'Functors   //
//    to encapsulate C and C++ Function Pointers'                             //
//                                                                            //
//    This program is free software; you can redistribute it and/or modify    //
//    it under the terms of the GNU General Public License as published by    //
//    the Free Software Foundation as version 2 of the License.               //
//                                                                            //
//    This program is distributed in the hope that it will be useful,         //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of          //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           //
//    GNU General Public License for more details.                            //
//                                                                            //
//    You should have received a copy of the GNU General Public License       //
//    along with this program; if not, write to the Free Software             //
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               //
//                                                                            //
//----------------------------------------------------------------------------//

#include <iostream>     // due to: cout

using namespace std;

//------------------------------------------------------------------------------------
// 4.2 How to Implement Functors

// abstract base class
class TFunctor
{
public:

   // two possible functions to call member function. virtual cause derived
   // classes will use a pointer to an object and a pointer to a member function
   // to make the function call
   virtual void operator()(const char* string)=0;  // call using operator
   virtual void Call(const char* string)=0;        // call using function
};


// derived template class
template <class TClass> class TSpecificFunctor : public TFunctor
{
private:
   void (TClass::*fpt)(const char*);   // pointer to member function
   TClass* pt2Object;                  // pointer to object

public:

   // constructor - takes pointer to an object and pointer to a member and stores
   // them in two private variables
   TSpecificFunctor(TClass* _pt2Object, void(TClass::*_fpt)(const char*))
      { pt2Object = _pt2Object;  fpt=_fpt; };

   // override operator "()"
   virtual void operator()(const char* string)
    { (*pt2Object.*fpt)(string);};              // execute member function

   // override function "Call"
   virtual void Call(const char* string)
     { (*pt2Object.*fpt)(string);};             // execute member function
};




//------------------------------------------------------------------------------------
// 4.3 Example of How to Use Functors

// dummy class A
class TClassA{
public:

   TClassA(){};
   void Display(const char* text) { cout << text << endl; };

   /* more of TClassA */
};

// dummy class B
class TClassB{
public:

   TClassB(){};
   void Display(const char* text) { cout << text << endl; };

   /* more of TClassB */
};


// main program
int main(int /*argc*/, char* /*argv[]*/)
{
   // 1. instantiate objects of TClassA and TClassB
   TClassA objA;
   TClassB objB;


   // 2. instantiate TSpecificFunctor objects ...
   //    a ) functor which encapsulates pointer to object and to member of TClassA
   TSpecificFunctor<TClassA> specFuncA(&objA, &TClassA::Display);

   //    b) functor which encapsulates pointer to object and to member of TClassB
   TSpecificFunctor<TClassB> specFuncB(&objB, &TClassB::Display);


   // 3. make array with pointers to TFunctor, the base class, and initialize it
   TFunctor* vTable[] = { &specFuncA, &specFuncB };


   // 4. use array to call member functions without the need of an object
   vTable[0]->Call("TClassA::Display called!");        // via function "Call"
   (*vTable[1])   ("TClassB::Display called!");        // via operator "()"


   // hit enter to terminate
   cout << endl << "Hit Enter to terminate!" << endl;
   cin.get();

   return 0;
}
