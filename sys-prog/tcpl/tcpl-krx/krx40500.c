#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<math.h>
#include <string.h>

#define MAXOP 100
#define NUMBER       0
#define IDENTIFIER   1
#define TRUE 1
#define FALSE 0

/* 

  The new additions deal with adding functions from math.h to the
  calculator.
  
  In anticipation of the following exercise the code deals with an 
  identifier in the following manner:
    
  If the identifier is recognised as one of the supported mathematical 
  functions then that function from the library is called. If the 
  identifier is not one of the supported functions, even if it is a
  valid function from math.h it is ignored.
      
  The main changes are the introduction of another define value   
  (IDENTIFIER) along with its associated case in the switch statement.  
  Getop has also been changed to deal with reading in alphabetical 
  characters.
       
  This is exercise 4-5 from Kernighan & Ritchie, page 79.
          
*/

int Getop(char s[]);
void push(double val);
double pop(void);
void showTop(void);
void duplicate(void);
void swapItems(void);
void clearStack();
void dealWithName(char s[]);

int main(void)
{
   int type;
   double op2;
   char s[MAXOP];
   int flag = TRUE;
   
   while((type = Getop(s)) != EOF)
   {
      switch(type)
      {
      case NUMBER:
         push(atof(s));
         break;
      case IDENTIFIER:
         dealWithName(s);
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
         clearStack();
      case '\n':
         printf("\n\t%.8g\n", pop());
         break;
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
      return val[--sp];
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

void clearStack(void)
{
   sp = 0;
}

/* deal with a string/name this may be either a maths function or for
future exercises: a variable */
void dealWithName(char s[])
{
   double op2;
   
   if( 0 == strcmp(s, "sin"))
      push(sin(pop()));
   else if( 0 == strcmp(s, "cos"))
      push(cos(pop()));
   else if (0 == strcmp(s, "exp"))
      push(exp(pop()));
   else if(!strcmp(s, "pow"))
   {
      op2 = pop();
      push(pow(pop(), op2));
   }
   else
      printf("%s is not a supported function.\n", s);
}

int getch(void);
void unGetch(int);

/* Getop: get next operator or numeric operand. */
int Getop(char s[])
{
   int i = 0;
   int c;
   int next;
   /*size_t len;*/
   
   /* Skip whitespace */
   while((s[0] = c = getch()) == ' ' || c == '\t')
      ;
   s[1] = '\0';
   
   if(isalpha(c))
   {
      i = 0;
      while(isalpha(s[i++] = c ))
         c = getch();     
      s[i - 1] = '\0';
      if(c != EOF)
         unGetch(c);
      return IDENTIFIER;
   }
   
   /* Not a number but may contain a unary minus. */
   if(!isdigit(c) && c != '.' && c != '-')
      return c;                 
   
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
      c = getch();
   
   while(isdigit(s[++i] = c))
      c = getch();
   if(c == '.')                 /* Collect fraction part. */
      while(isdigit(s[++i] = c = getch()))
         ;
      s[i] = '\0';
      if(c != EOF)
         unGetch(c);
      return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE];
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
