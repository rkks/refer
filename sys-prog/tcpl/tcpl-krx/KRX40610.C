#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXOP      100
#define NUMBER       0
/* 4-6 these are new for this exercise*/
#define IDENTIFIER   1
#define ENDSTRING    2
/* 4-6 end of new stuff */
#define TRUE         1
#define FALSE        0
#define MAX_ID_LEN  32
#define MAXVARS     30

/* 
The new additions deal with adding variables to the calculator.

  If the identifier is recognised as one of the supported mathematical 
  functions then that function from the library is called. If the 
  identifier is not one of the supported functions, even if it is a
  valid function from math.h it is ignored.
  
  This is a class 1 solution as it uses structures which are not 
  introduced until Chapter 6. This allows the use of "normal" names for
  variables rather than the suggested single letter though any 
  identifier is limited to 31 characters.
    
  The main changes are:
      
    1. The introduction of two more define values (IDENTIFIER, 
       ENDSTRING) along with associated cases in the switch statement. 
    2. Getop has also been changed to deal with reading in alphabetical 
       characters and coping with the '=' sign.
    3. A structure to hold the variable name and value.
    4. Another case in the switch statement to deal with the '=' sign.
    5. Altering the clearStack function to clear the array of structs as
       well as the stack.
    6. The '<' operator now prints the last accessed variable.
        
  Improvements:
  The code could be made class 0 by the use of "parallel" arrays for the
  names and values rather than a struct but this would be messy and is 
  the situation that structs were made for.
  The use of a binary tree together with dynamically allocated memory
  would allow the arbitrary limit of 30 variables to be avoided. This 
  would still be a class 1 solution. 
         
  This is exercise 4-6 from Kernighan & Ritchie, page 79.               
*/

/* 4-6 this is new for this program */
struct varType{
   char name[MAX_ID_LEN];
   double val;
};
/* 4-6 End of new stuff */

int Getop(char s[]);
void push(double val);
double pop(void);
void showTop(void);
void duplicate(void);
void swapItems(void);

/* 4-6 this is new for this program */
/* Changed clearStack(void) to clearStacks(struct varType var[])*/
void clearStacks(struct varType var[]);
void dealWithName(char s[],  struct varType var[]);
void dealWithVar(char s[], struct varType var[]);

int pos = 0;
struct varType last;

/* 4-6 End of new stuff */

int main(void)
{
   int type;
   double op2;
   char s[MAXOP];
   struct varType var[MAXVARS];
   
   /* Use the new function here */
   clearStacks(var);
   
   while((type = Getop(s)) != EOF)
   {
      switch(type)
      {
      case NUMBER:
         push(atof(s));
         break;
      case IDENTIFIER:
         dealWithName(s, var);
         break;
      case '+':
         push(pop() + pop());
         break;
      case '*':
         push(pop() * pop());
         break;
      case '-':
         op2 = pop();
         push(pop()- op2);
         break;
      case '/':
         op2 = pop();
         if(op2)
            push(pop() / op2);
         else
            printf("\nError: division by zero!");
         break;
      case '%':
         op2 = pop();
         if(op2)
            push(fmod(pop(), op2));
         else
            printf("\nError: division by zero!");
         break;
      case '?':
         showTop();
         break;
      case '#':
         duplicate();
         break;
      case '~':
         swapItems();
         break;
      case '!':
         clearStacks(var);
         break;
      case '\n':
         printf("\n\t%.8g\n", pop());
         break;
         /* 4-6 this is new for this program */
      case ENDSTRING:
         break;
      case '=':
         pop();
         var[pos].val = pop();
         last.val = var[pos].val;
         push(last.val);
         break;
      case '<':
         printf("The last variable used was: %s (value == %g)\n",
                                            last.name, last.val);
         break;
         /* 4-6 End of new stuff */
      default:
         printf("\nError: unknown command %s.\n", s);
         break;
      }
   }
   return EXIT_SUCCESS;
}

#define MAXVAL 100

int sp = 0;          /* Next free stack position. */
double val[MAXVAL];  /* value stack. */

/* push: push f onto stack. */
void push(double f)
{
   if(sp < MAXVAL)
      val[sp++] = f;
   else
      printf("\nError: stack full can't push %g\n", f);
}

/*pop: pop and return top value from stack.*/
double pop(void)
{
   if(sp > 0)
   {
      return val[--sp];
   }
   else
   {
      printf("\nError: stack empty\n");
      return 0.0;
   }
}

void showTop(void)
{
   if(sp > 0)
      printf("Top of stack contains: %8g\n", val[sp-1]);
   else
      printf("The stack is empty!\n");
}

/*
Alternatively:
void showTop(void)
{
double item = pop();
printf("Top of stack contains: %8g\n", item);
push(item);
}  
*/

void duplicate(void)
{
   double temp = pop();
   
   push(temp);
   push(temp);
}

void swapItems(void)
{
   double item1 = pop();
   double item2 = pop();
   
   push(item1);
   push(item2);
}

/* 4-6 this is new for this program */
/* Altered to clear both the main stack and that of the variable
structure */
void clearStacks(struct varType var[])
{
   int i;
   
   /* Clear the main stack by setting the pointer to the bottom. */
   sp = 0;
   
   /* Clear the variables by setting the initial element of each name
   to the terminating character. */
   for( i = 0; i < MAXVARS; ++i)
   {
      var[i].name[0] = '\0';
      var[i].val = 0.0;
   }
}

/* a string/name may be either a maths function or a variable */
void dealWithName(char s[], struct varType var[])
{
   double op2;
   
   if(!strcmp(s, "sin"))
      push(sin(pop()));
   else if(!strcmp(s, "cos"))
      push(cos(pop()));
   else if (!strcmp(s, "exp"))
      push(exp(pop()));
   else if(!strcmp(s, "pow"))
   {
      op2 = pop();
      push(pow(pop(), op2));
   }
   /* Finally if it isn't one of the supported maths functions we have a 
      variable to deal with. */
   else
   {
      dealWithVar(s, var);
   }
}

/* Our identifier is not one of the supported maths function so we have 
   to regard it as an identifier. */
void dealWithVar(char s[], struct varType var[])
{
   int i = 0;
   
   while(var[i].name[0] != '\0' && i < MAXVARS-1)
   {
      if(!strcmp(s, var[i].name))
      {
         strcpy(last.name, s);
                   last.val = var[i].val;
         push(var[i].val);
         pos = i;
         return;
      }
      i++;
   }
   
   /* variable name not found so add it */
   strcpy(var[i].name, s);
   /* And save it to the last variable */
   strcpy(last.name, s);
   push(var[i].val);
   pos = i;
}
/* 4-6 End of new stuff */

int getch(void);
void unGetch(int);

/* Getop: get next operator or numeric operand. */
int Getop(char s[])
{
   int i = 0;
   int c;
   int next;
   
   /* Skip whitespace */
   while((s[0] = c = getch()) == ' ' || c == '\t')
   {
      ;
   }
   s[1] = '\0';
   
   if(isalpha(c))
   {
      i = 0;
      while(isalpha(s[i++] = c ))
      {
         c = getch();     
      }
      s[i - 1] = '\0';
      if(c != EOF)
         unGetch(c);
      return IDENTIFIER;
   }
   
   /* Not a number but may contain a unary minus. */
   if(!isdigit(c) && c != '.' && c != '-')
   {
      /* 4-6 Deal with assigning a variable. */
      if('=' == c && '\n' == (next = getch()))
      {
         unGetch('\0');
         return c;
      }
      if('\0' == c)
         return ENDSTRING;

      return c;                 
   }
   
   if(c == '-')
   {
      next = getch();
      if(!isdigit(next) && next != '.')
      {
         return c;
      }
      c = next;
   }
   else
   {
      c = getch();
   }
   
   while(isdigit(s[++i] = c))
   {
      c = getch();
   }
   if(c == '.')                 /* Collect fraction part. */
   {
      while(isdigit(s[++i] = c = getch()))
         ;
   }
   s[i] = '\0';
   if(c != EOF)
      unGetch(c);
   return NUMBER;
}

#define BUFSIZE 100

int buf[BUFSIZE];
int bufp = 0;

/* Getch: get a ( possibly pushed back) character. */
int getch(void)
{
   return (bufp > 0) ? buf[--bufp]: getchar();
}

/* unGetch: push character back on input. */
void unGetch(int c)
{
   if(bufp >= BUFSIZE)
      printf("\nUnGetch: too many characters\n");
   else
      buf[bufp++] = c;
}

