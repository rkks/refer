#include "hoc.h"
#include "y.tab.h"

#define	NSTACK	256
static	Datum	stack[NSTACK];	/* the stack */
static	Datum	*stackp;	/* next free spot on stack */

#define	NPROG	2000
Inst	prog[NPROG];	/* the machine */
Inst	*progp;		/* next free spot for code generation */
Inst	*pc;		/* program counter during execution */

initcode()	/* initialize for code generation */
{
	stackp = stack;
	progp = prog;
}

push(d)		/* push d onto stack */
	Datum d;
{
	if (stackp >= &stack[NSTACK])
		execerror("stack overflow", (char *) 0);
	*stackp++ = d;
}

Datum pop()	/* pop and return top elem from stack */
{
	if (stackp <= stack)
		execerror("stack underflow", (char *) 0);
	return *--stackp;
}

constpush()	/* push constant onto stack */
{
	Datum d;
	d.val = ((Symbol *)*pc++)->u.val;
	push(d);
}

varpush()	/* push variable onto stack */
{
	Datum d;
	d.sym = (Symbol *)(*pc++);
	push(d);
}

bltin()		/* evaluate built-in on top of stack */
{
	Datum d;
	d = pop();
	d.val = (*(double (*)())(*pc++))(d.val);
	push(d);
}

eval()		/* evaluate variable on stack */
{
	Datum d;
	d = pop();
	if (d.sym->type == UNDEF)
		execerror("undefined variable", d.sym->name);
	d.val = d.sym->u.val;
	push(d);
}

add()		/* add top two elems on stack */
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val += d2.val;
	push(d1);
}

sub()	/* subtract top of stack from next */
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val -= d2.val;
	push(d1);
}

mul()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val *= d2.val;
	push(d1);
}

div()
{
	Datum d1, d2;
	d2 = pop();
	if (d2.val == 0.0)
		execerror("division by zero", (char *) 0);
	d1 = pop();
	d1.val /= d2.val;
	push(d1);
}

negate()
{
	Datum d;
	d = pop();
	d.val = -d.val;
	push(d);
}

power()
{
	Datum d1, d2;
	extern double Pow();
	d2 = pop();
	d1 = pop();
	d1.val = Pow(d1.val, d2.val);
	push(d1);
}
assign()	/* assign top value to next value */
{
	Datum d1, d2;
	d1 = pop();
	d2 = pop();
	if (d1.sym->type != VAR && d1.sym->type != UNDEF)
		execerror("assignment to non-variable",
			d1.sym->name);
	d1.sym->u.val = d2.val;
	d1.sym->type = VAR;
	push(d2);
}

print()		/* pop top value from stack, print it */
{
	Datum d;
	d = pop();
	printf("\t%.8g\n", d.val);
}

Inst *code(f)	/* install one instruction or operand */
	Inst f;
{
	Inst *oprogp = progp;
	if (progp >= &prog[NPROG])
		execerror("program too big", (char *) 0);
	*progp++ = f;
	return oprogp;
}

execute(p)	/* run the machine */
	Inst *p;
{
	for (pc = p; *pc != STOP; )
		(*(*pc++))();
}
