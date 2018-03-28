/*
 * Solution to exercise 5-10 in K&R2:
 *
 *	Write the program expr, which evaluates a reverse Polish expression
 *	from the command line, where each operator or operand is a separate 
 *	argument. For example,
 *
 *		expr 2 3 4 + *
 *
 *	evaluates 2*(3+4).
 *
 * This is very similar to the program in 4.3 (and should ideally have been
 * a modification of that).
 *
 * Feel free to modify and copy freely.
 *
 * Lars Wirzenius <liw@iki.fi>
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 1024

double stack[STACK_SIZE];
int stack_height = 0;

void panic(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

void push(double value) {
	if (stack_height == STACK_SIZE)
		panic("stack is too high!");
	stack[stack_height] = value;
	++stack_height;
}

double pop(void) {
	if (stack_height == 0)
		panic("stack is empty!");
	return stack[--stack_height];
}

int main(int argc, char **argv) {
	int i;
	double value;
	
	for (i = 1; i < argc; ++i) {
		switch (argv[i][0]) {
		case '\0':
			panic("empty command line argument");
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			push(atof(argv[i]));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '-':
			value = pop();
			push(pop() - value);
			break;
		case '*':
			push(pop() * pop());
			break;
		case '/':
			value = pop();
			push(pop() / value);
			break;
		default:
			panic("unknown operator");
			break;
		}
	}

	printf("%g\n", pop());
	return 0;
}
