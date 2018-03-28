// This code was compiled and tested with Borland C++ Builder 5.0.
#pragma hdrstop       // Borland C++ Builder specific
#pragma argsused      // do.

//----------------------------------------------------------------------------//
//    Copyright (c) 2000-2005 by Lars Haendel                                 //
//    Home: http://www.newty.de                                               //
//    Last modified: 06 january 2005                                          //
//                                                                            //
//    Example code for 'The Function Pointer Tutorials', Chapter 3 'How       //
//    to Implement Callbacks in C and C++'                                    //
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


/*
void qsort(void* field, size_t nElements, size_t sizeOfAnElement,
                 int(_USERENTRY *cmpFunc)(const void*, const void*));



void qsort( ... , int(_USERENTRY *cmpFunc)(const void*, const void*))
{
   // sort algorithm  - note: item1 and item2 are void-pointers

   int bigger=cmpFunc(item1, item2);  // make callback

   // use the result
}
*/



//------------------------------------------------------------------------------------
// 3.3 How to make a callback in C by the means of the sort function qsort

#include <stdlib>       // due to:  qsort
#include <time>         //          randomize
#include <stdio>        //          printf
#include <iostream>     //          cout


// comparison-function for the sort-algorithm
// two items are taken by void-pointer, converted and compared
int CmpFunc(const void* _a, const void* _b)
{
   // you've got to explicitly cast to the correct type
   const float* a = (const float*) _a;
   const float* b = (const float*) _b;

   if(*a > *b) return 1;      // first item is bigger than the second one -> return 1
   else
      if(*a == *b) return  0; // equality -> return 0
      else         return -1; // second item is bigger than the first one -> return -1
}


// example for the use of qsort()
void QSortExample()
{
   float field[100];

   ::randomize();                     // initialize random-number-generator
   for(int c=0;c<100;c++)             // randomize all elements of the field
      field[c]=random(99);

   // sort using qsort()
   qsort((void*) field, /*number of items*/ 100, /*size of an item*/ sizeof(field[0]),
         /*comparison-function*/ CmpFunc);

   // display first ten elements of the sorted field
   printf("The first ten elements of the sorted field are ...\n");
   for(int c=0;c<10;c++)
      printf("element #%d contains %.0f\n", c+1, field[c]);
   printf("\n");
}


//------------------------------------------------------------------------------------
// 3.5 Example A: Callback to member function using an additional argument
// Task: The function 'DoItA' makes something which implies a callback to
//       the member function 'Display'. Therefore the wrapper function
//       'Wrapper_To_Call_Display is used.

class TClassA
{
public:

   void Display(const char* text) { std::cout << text << std::endl; };
   static void Wrapper_To_Call_Display(void* pt2Object, char* text);

   /* more of TClassA */
};


// static wrapper function to be able to callback the member function Display()
void TClassA::Wrapper_To_Call_Display(void* pt2Object, char* string)
{
    // explicitly cast to a pointer to TClassA
    TClassA* mySelf = (TClassA*) pt2Object;

    // call member
    mySelf->Display(string);
}


// function does something which implies a callback
// note: of course this function can also be a member function
void DoItA(void* pt2Object, void (*pt2Function)(void* pt2Object, char* text))
{
   /* do something */

   pt2Function(pt2Object, "hi, i'm calling back using an argument ;-)");// make callback
}


// execute example code
void Callback_Using_Argument()
{
   // 1. instantiate object of TClassA
   TClassA objA;

   // 2. call 'DoItA' for <objA>
   DoItA((void*) &objA, TClassA::Wrapper_To_Call_Display);
}




//------------------------------------------------------------------------------------
// 3.5 Example B: Callback to member function using a global variable
// Task: The function 'DoItB' makes something which implies a callback to
//       the member function 'Display'. Therefore the wrapper function
//       'Wrapper_To_Call_Display is used.

void* pt2Object;       // global variable which points to an arbitrary object

class TClassB
{
public:

   void Display(const char* text) { std::cout << text << std::endl; };
   static void Wrapper_To_Call_Display(char* text);

   /* more of TClassB */
};


// static wrapper function to be able to callback the member function Display()
void TClassB::Wrapper_To_Call_Display(char* string)
{
    // explicitly cast global variable <pt2Object> to a pointer to TClassB
    // warning: <pt2Object> MUST point to an appropriate object!
    TClassB* mySelf = (TClassB*) pt2Object;

    // call member
    mySelf->Display(string);
}


// function does something which implies a callback
// note: of course this function can also be a member function
void DoItB(void (*pt2Function)(char* text))
{
   /* do something */

   pt2Function("hi, i'm calling back using a global ;-)");   // make callback
}


// execute example code
void Callback_Using_Global()
{
   // 1. instantiate object of TClassB
   TClassB objB;


   // 2. assign global variable which is used in the static wrapper function
   // important: never forget to do this!!
   pt2Object = (void*) &objB;


   // 3. call 'DoItB' for <objB>
   DoItB(TClassB::Wrapper_To_Call_Display);
}


//------------------------------------------------------------------------------------
// main program - execute all the examples
int main(int /*argc*/, char* /*argv[]*/)
{
   // execute examples
   QSortExample();
   Callback_Using_Argument();
   Callback_Using_Global();

   // wait for user input to terminate
   std::cout << std::endl << "Hit Enter to terminate!" << std::endl;
   std::cin.get();

   return 0;
}
