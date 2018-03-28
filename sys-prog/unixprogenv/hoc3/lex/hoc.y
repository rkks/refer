%{
#include "hoc.h"
extern	double	Pow();
%}
%union {
	double	val;	/* actual value */
	Symbol	*sym;	/* symbol table pointer */
}
%token	<val>	NUMBER
%token	<sym>	VAR BLTIN UNDEF
%type	<val>	expr asgn
%right	'='
%left	'+' '-'
%left	'*' '/'
%left	UNARYMINUS
%right	'^'	/* exponentiation */
%%
list:	  /* nothing */
	| list '\n'
	| list asgn '\n'
	| list expr '\n'	{ printf("\t%.8g\n", $2); }
	| list error '\n'	{ yyerrok; }
	;
asgn:	  VAR '=' expr { $$=$1->u.val=$3; $1->type = VAR; }
	;
expr:	  NUMBER
	| VAR {	if ($1->type == UNDEF)
		    execerror("undefined variable", $1->name);
		$$ = $1->u.val; }
	| asgn
	| BLTIN '(' expr ')'	{ $$ = (*($1->u.ptr))($3); }
	| expr '+' expr	{ $$ = $1 + $3; }
	| expr '-' expr	{ $$ = $1 - $3; }
	| expr '*' expr	{ $$ = $1 * $3; }
	| expr '/' expr	{
		if ($3 == 0.0)
			execerror("division by zero", "");
		$$ = $1 / $3; }
	| expr '^' expr	{ $$ = Pow($1, $3); }
	| '(' expr ')'	{ $$ = $2; }
	| '-' expr  %prec UNARYMINUS  { $$ = -$2; }
	;
%%
	/* end of grammar */
#include <stdio.h>
#include <ctype.h>
char	*progname;
int	lineno = 1;
#include <setjmp.h>
jmp_buf begin;

main(argc, argv)	/* hoc3 */
	char *argv[];
{
	progname = argv[0];
	init();
	setjmp(begin);
	yyparse();
}

yyerror(s)
	char *s;
{
	warning(s, (char *)0);
}
execerror(s, t)
	char *s, *t;
{
	warning(s, t);
	longjmp(begin, 0);
}
warning(s, t)
	char *s, *t;
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t && *t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %d\n", lineno);
}
