// This code was compiled and tested with Borland C++ Builder 5.0.
#pragma hdrstop       // Borland C++ Builder specific
#pragma argsused      // do.

//----------------------------------------------------------------------------//
//    Copyright (c) 2000-2005 by Lars Haendel                                 //
//    Home: http://www.newty.de                                               //
//    Last modified: 06 january 2005                                          //
//                                                                            //
//    Example code for 'The Function Pointer Tutorials', Chapter 1 and 2      //
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
//                                                                            //
//----------------------------------------------------------------------------//


#include <iostream>        // due to: cout
#include <stdio>			   //         sprintf

using namespace std;

//------------------------------------------------------------------------------------
// 1.2 Introductory Example or How to Replace a Switch-Statement
// Task: Perform one of the four basic arithmetic operations specified by the
//       characters '+', '-', '*' or '/'.


// The four arithmetic operations ... one of these functions is selected
// at runtime with a swicth or a function pointer
float Plus    (float a, float b) { return a+b; }
float Minus   (float a, float b) { return a-b; }
float Multiply(float a, float b) { return a*b; }
float Divide  (float a, float b) { return a/b; }


// Solution with a switch-statement - <opCode> specifies which operation to execute
void Switch(float a, float b, char opCode)
{
   float result;

   // execute operation
   switch(opCode)
   {
      case '+' : result = Plus     (a, b); break;
      case '-' : result = Minus    (a, b); break;
      case '*' : result = Multiply (a, b); break;
      case '/' : result = Divide   (a, b); break;
   }

   cout << "Switch: 2+5=" << result << endl;         // display result
}


// Solution with a function pointer - <pt2Func> is a function pointer and points to
// a function which takes two floats and returns a float. The function pointer
// "specifies" which operation shall be executed.
void Switch_With_Function_Pointer(float a, float b, float (*pt2Func)(float, float))
{
   float result = pt2Func(a, b);    // call using function pointer

   cout << "Switch replaced by function pointer: 2-5=";  // display result
   cout << result << endl;
}


// Execute example code
void Replace_A_Switch()
{
   cout << endl << "Executing function 'Replace_A_Switch'" << endl;

   Switch(2, 5, /* '+' specifies function 'Plus' to be executed */ '+');
   Switch_With_Function_Pointer(2, 5, /* pointer to function 'Minus' */ &Minus);
}




//------------------------------------------------------------------------------------
// 2.1-4  Define, Assign an Address and Call a Function using a Function Pointer

// C   -  definition of 'DoIt' and 'DoMore'
int DoIt  (float a, char b, char c){ printf("DoIt\n");   return a+b+c; }
int DoMore(float a, char b, char c){ printf("DoMore\n"); return a-b+c; }


// C++  -  definition of class TMyClass
class TMyClass
{
public:
   int DoIt(float a, char b, char c){ cout << "TMyClass::DoIt"<< endl; return a+b+c;};
   int DoMore(float a, char b, char c) const
         { cout << "TMyClass::DoMore" << endl; return a-b+c; };

   /* more of TMyClass */
};


// Illustrate how to define, assign an address, compare or call a function using a
// function pointer
void Syntax_Of_Function_Pointers()
{
   cout << endl << "Executing function 'Syntax_Of_Function_Pointers'" << endl;

   // 2.1 define a function pointer and initialize to NULL
   int (*pt2Function)(float, char, char) = NULL;                        // C
   int (TMyClass::*pt2Member)(float, char, char) = NULL;                // C++
   int (TMyClass::*pt2ConstMember)(float, char, char) const = NULL;     // C++


   // 2.3 Assign an address to the function pointer
   //     Note: Although you may ommit the address operator on most compilers
   //     you should always use the correct way in order to write portable code.

   // C
   pt2Function = DoIt;      // short form
   pt2Function = &DoMore;   // correct assignment using address operator

   // C++
   pt2ConstMember = &TMyClass::DoMore; // correct assignment using address operator
   pt2Member = &TMyClass::DoIt; // note: <pt2Member> may also legally point to &DoMore


   // 2.4 Comparing function pointers
   // C
   if(pt2Function >0){                           // check if initialized
      if(pt2Function == &DoIt)
         printf("Pointer points to DoIt\n"); }
   else
      printf("Pointer not initialized!!\n");

   // C++
   if(pt2ConstMember == &TMyClass::DoMore)
      cout << "Pointer points to TMyClass::DoMore" << endl;


   // 2.5 Calling a function using a function pointer
   int result1 = pt2Function   (12, 'a', 'b');          // C short way
   int result2 = (*pt2Function)(12, 'a', 'b');          // C correct way

   TMyClass instance1;
   int result3 = (instance1.*pt2Member)(12, 'a', 'b');  // C++

   TMyClass* instance2 = new TMyClass;
   int result4 = (instance2->*pt2Member)(12, 'a', 'b'); // C++, instance2 is a pointer
   delete instance2;

   cout << result1 << " " << result2 << " " << result3 << " " << result4 << endl;
}




//------------------------------------------------------------------------------------
// 2.6 How to Pass a Function Pointer

// <pt2Func> is a pointer to a function which returns an int and takes a float and two char
void PassPtr(int (*pt2Func)(float, char, char))
{
   int result = (*pt2Func)(12, 'a', 'b');     // call using function pointer
   cout << result << endl;
}

// execute example code - 'DoIt' is a suitable function like defined above in 2.1-4
void Pass_A_Function_Pointer()
{
   cout << endl << "Executing 'Pass_A_Function_Pointer'" << endl;
   PassPtr(&DoIt);
}




//------------------------------------------------------------------------------------
// 2.7 How to Return a Function Pointer
//     'Plus' and 'Minus' are defined above. They return a float and take two float


// Direct solution: Function takes a char and returns a pointer to a
// function which is taking two floats and returns a float. <opCode>
// specifies which function to return
float (*GetPtr1(const char opCode))(float, float)
{
   if(opCode == '+')
      return &Plus;
   else
      return &Minus; // default if invalid operator was passed
}


// Solution using a typedef: Define a pointer to a function which is taking
// two floats and returns a float
typedef float(*pt2Func)(float, float);

// Function takes a char and returns a function pointer which is defined
// with the typedef above. <opCode> specifies which function to return
pt2Func GetPtr2(const char opCode)
{
   if(opCode == '+')
      return &Plus;
   else
      return &Minus; // default if invalid operator was passed
}


// Execute example code
void Return_A_Function_Pointer()
{
   cout << endl << "Executing 'Return_A_Function_Pointer'" << endl;

   // define a function pointer and initialize it to NULL
   float (*pt2Function)(float, float) = NULL;

   pt2Function=GetPtr1('+');   // get function pointer from function 'GetPtr1'
   cout << (*pt2Function)(2, 4) << endl;   // call function using the pointer


   pt2Function=GetPtr2('-');   // get function pointer from function 'GetPtr2'
   cout << (*pt2Function)(2, 4) << endl;   // call function using the pointer
}



//------------------------------------------------------------------------------------
// 2.8 How to Use Arrays of Function Pointers

// C ---------------------------------------------------------------------------------

// type-definition: 'pt2Function' now can be used as type
typedef int (*pt2Function)(float, char, char);

// illustrate how to work with an array of function pointers
void Array_Of_Function_Pointers()
{
   printf("\nExecuting 'Array_Of_Function_Pointers'\n");

   // define arrays and ini each element to NULL, <funcArr1> and <funcArr2> are arrays
   // with 10 pointers to functions which return an int and take a float and two char

   // first way using the typedef
   pt2Function funcArr1[10] = {NULL};

   // 2nd way directly defining the array
   int (*funcArr2[10])(float, char, char) = {NULL};


   // assign the function's address - 'DoIt' and 'DoMore' are suitable functions
   // like defined above in 2.1-4
   funcArr1[0] = funcArr2[1] = &DoIt;
   funcArr1[1] = funcArr2[0] = &DoMore;

   /* more assignments */

   // calling a function using an index to address the function pointer
   printf("%d\n", funcArr1[1](12, 'a', 'b'));         //  short form
   printf("%d\n", (*funcArr1[0])(12, 'a', 'b'));      // "correct" way of calling
   printf("%d\n", (*funcArr2[1])(56, 'a', 'b'));
   printf("%d\n", (*funcArr2[0])(34, 'a', 'b'));
}


// C++ -------------------------------------------------------------------------------

// type-definition: 'pt2Member' now can be used as type
typedef int (TMyClass::*pt2Member)(float, char, char);

// illustrate how to work with an array of member function pointers
void Array_Of_Member_Function_Pointers()
{
   cout << endl << "Executing 'Array_Of_Member_Function_Pointers'" << endl;

   // define arrays and ini each element to NULL, <funcArr1> and <funcArr2> are
   // arrays with 10 pointers to member functions which return an int and take
   // a float and two char

   // first way using the typedef
   pt2Member funcArr1[10] = {NULL};

   // 2nd way of directly defining the array
   int (TMyClass::*funcArr2[10])(float, char, char) = {NULL};


   // assign the function's address - 'DoIt' and 'DoMore' are suitable member
   //  functions of class TMyClass like defined above in 2.1-4
   funcArr1[0] = funcArr2[1] = &TMyClass::DoIt;
   funcArr1[1] = funcArr2[0] = &TMyClass::DoMore;
   /* more assignments */

   // calling a function using an index to address the member function pointer
   // note: an instance of TMyClass is needed to call the member functions
   TMyClass instance;
   cout << (instance.*funcArr1[1])(12, 'a', 'b') << endl;
   cout << (instance.*funcArr1[0])(12, 'a', 'b') << endl;
   cout << (instance.*funcArr2[1])(34, 'a', 'b') << endl;
   cout << (instance.*funcArr2[0])(89, 'a', 'b') << endl;
}



//------------------------------------------------------------------------------------
// main program: execute all examples
int main(int /*argc*/, char* /*argv[]*/)
{
   Replace_A_Switch();
   Syntax_Of_Function_Pointers();
   Pass_A_Function_Pointer();
   Return_A_Function_Pointer();
   Array_Of_Function_Pointers();
   Array_Of_Member_Function_Pointers();

   cin.get();                                // wait for user input to terminate
   return 0;
}
