/* Program 3.1: Function definition
 * The mechanism that we use in C to implement new operations on 
 * data is the *function definition*, illustrated here.
 *
 * All functions have a list of *arguments* and possibly a *return
 * value*. The function `lg` here has one argument and a return
 * value, each of type `int`. The function `main` has neither
 * arguments nor return value.
 *
 * We *declare* the function by giving its name and the types of
 * its return values. The first line of code here references a
 * system file that contains declarations of system functions such
 * as `printf`. The second line of code is a declaration for `lg`.
 * The declaration is optional if the function defined before it
 * is used, as is the case with `main`. The declaration provides
 * the information necessary for other functions to *call* or
 * *invoke* the function, using arguments of the proper type. The
 * calling function can use the function in an expression, in the
 * same way as it uses variables of the return-value type.
 *
 * We *define* functions with C code. All C programs include a
 * definition of the function `main`, and this code also defines
 * `lg`. In a function definition, we give names to the arguments
 * (which we refer to as *parameters*) and express the computation
 * in terms of those names, as if they were local variables. When
 * the function is invoked, these variables are initialized with
 * the values of the arguments and the function code is executed.
 * The *return* statement is the instruction to end execution of
 * the function and provide the return value to the calling
 * function. In principle, the calling function is not to be
 * otherwise affected, though we shall see many exceptions to this
 * principle.
 *
 * The separation of definition and declaration provides
 * flexibility in organizing programs. For example, both could be
 * in separate files. Or, in a simple program like this one, we
 * could put the definition of `lg` before the definition of
 * `main` and omit its declaration.
 */
#include <stdio.h>
int lg(int);

int main() {
  int i, N;

  for (i = 1, N = 10; i <= 6; ++i, N *= 10)
    printf("%7d %2d %9d\n", N, lg(N), N*lg(N));
}

int lg(int N) {
  int i;

  for (i = 0; N > 0; ++i, N /= 2)
    ;
  return i;
}
