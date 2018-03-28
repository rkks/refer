#include "hoc.h"
#include "y.tab.h"

#define	NSTACK	256

static Datum stack[NSTACK];
static Datum *stackp;

#define	NPROG	2000
Inst prog[NPROG];
static Inst *pc;
Inst *progp;

initcode()
{
	progp = prog;
	stackp = stack;
}

push(d)
	Datum d;
{
	if (stackp >= &stack[NSTACK])
		execerror("stack too deep", (char *)0);
	*stackp++ = d;
}

Datum
pop()
{
	if (stackp == stack)
		execerror("stack underflow", (char *)0);
	return *--stackp;
}

constpush()
{
	Datum d;
	d.val = ((Symbol *)*pc++)->u.val;
	push(d);
}

varpush()
{
	Datum d;
	d.sym = (Symbol *)(*pc++);
	push(d);
}

whilecode()
{
	Datum d;
	Inst *savepc = pc;	/* loop body */

	execute(savepc+2);	/* condition */
	d = pop();
	while (d.val) {
		execute(*((Inst **)(savepc)));	/* body */
		execute(savepc+2);
		d = pop();
	}
	pc = *((Inst **)(savepc+1));  /* next statement */
}

ifcode()
{
	Datum d;
	Inst *savepc = pc;	/* then part */

	execute(savepc+3);	/* condition */
	d = pop();
	if (d.val)
		execute(*((Inst **)(savepc)));
	else if (*((Inst **)(savepc+1))) /* else part? */
		execute(*((Inst **)(savepc+1)));
	pc = *((Inst **)(savepc+2));	 /* next stmt */
}

bltin()
{
	Datum d;
	d = pop();
	d.val = (*(double (*)())(*pc++))(d.val);
	push(d);
}

eval() /* Evaluate variable on stack */
{
	Datum d;
	d = pop();
	if (d.sym->type != VAR && d.sym->type != UNDEF)
		execerror("attempt to evaluate non-variable", d.sym->name);
	if (d.sym->type == UNDEF)
		execerror("undefined variable", d.sym->name);
	d.val = d.sym->u.val;
	push(d);
}

add()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val += d2.val;
	push(d1);
}

sub()
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
		execerror("division by zero", (char *)0);
	d1 = pop();
	d1.val /= d2.val;
	push(d1);
}

negate()
{
	Datum d;
	d = pop();
	d.val =  -d.val;
	push(d);
}

gt()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val > d2.val);
	push(d1);
}

lt()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val < d2.val);
	push(d1);
}

ge()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val >= d2.val);
	push(d1);
}

le()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val <= d2.val);
	push(d1);
}

eq()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val == d2.val);
	push(d1);
}

ne()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val  !=  d2.val);
	push(d1);
}

and()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val != 0.0 && d2.val != 0.0);
	push(d1);
}

or()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val != 0.0 || d2.val != 0.0);
	push(d1);
}

not()
{
	Datum d;
	d = pop();
	d.val = (double)(d.val == 0.0);
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

assign()
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

print()
{
	Datum d;
	d = pop();
	printf("\t%.8g\n", d.val);
}

prexpr()	/* print numeric value */
{
	Datum d;
	d = pop();
	printf("%.8g\n", d.val);
}

Inst *code(f)	/* install one instruction or operand */
	Inst f;
{
	Inst *oprogp = progp;
	if (progp >= &prog[NPROG])
		execerror("expression too complicated", (char *)0);
	*progp++ = f;
	return oprogp;
}

execute(p)
	Inst *p;
{
	for (pc = p; *pc != STOP; )
		(*(*pc++))();
}

