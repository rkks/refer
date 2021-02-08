/* Exercise 4.1: Give a definition for `Item` and `eq` that might
 * be used for floating-point nmbers, where two floating-point
 * numbers are considered to be equal if the absolute value of
 * their difference divided by the larger (in absolute value) of
 * the two numbers is less than 10^-6.
 */
typedef float Item;
#define eq(A, B) ((A>B?(A-B)/(A>0?A:-A):(B-A)/(B>0?B:-B)) < 1e-6)
