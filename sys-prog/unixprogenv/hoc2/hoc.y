%{
double	mem[26];	/* memory for variables 'a'..'z' */
%}
%union {		/* stack type */
	double	val;	/* actual value */
	int	index;	/* index into mem[] */
}
%token	<val>	NUMBER
%token	<index>	VAR
%type	<val>	expr
%right	'='
%left	'+' '-'
%left	'*' '/'
%left	UNARYMINUS
%%
list:	  /* nothing */
	| list '\n'
	| list expr '\n'	{ printf("\t%.8g\n", $2); }
	| list error '\n'	{ yyerrok; }
	;
expr:	  NUMBER
	| VAR		{ $$ = mem[$1]; }
	| VAR '=' expr	{ $$ = mem[$1] = $3; }	
	| expr '+' expr	{ $$ = $1 + $3; }
	| expr '-' expr	{ $$ = $1 - $3; }
	| expr '*' expr	{ $$ = $1 * $3; }
	| expr '/' expr	{
		if ($3 == 0.0)
			execerror("division by zero", "");
		$$ = $1 / $3; }
	| '(' expr ')'	{ $$ = $2; }
	| '-' expr  %prec UNARYMINUS  { $$ = -$2; }
	;
%%
	/* end of grammar */

#include <stdio.h>
#include <ctype.h>
char	*progname;
int	lineno = 1;
#include <signal.h>
#include <setjmp.h>
jmp_buf	begin;

main(argc, argv)	/* hoc2 */
	char *argv[];
{
	int fpecatch();

	progname = argv[0];
	setjmp(begin);
	signal(SIGFPE, fpecatch);
	yyparse();
}

yylex()		/* hoc2 */
{
	int c;

	while ((c=getchar()) == ' ' || c == '\t')
		;
	if (c == EOF)
		return 0;
	if (c == '.' || isdigit(c)) {	/* number */
		ungetc(c, stdin);
		scanf("%lf", &yylval.val);
		return NUMBER;
	}
	if (islower(c)) {
		yylval.index = c - 'a';	/* ASCII only */
		return VAR;
	}
	if (c == '\n')
		lineno++;
	return c;
}

yyerror(s)	/* report compile-time error */
	char *s;
{
	warning(s, (char *)0);
}

execerror(s, t)	/* recover from run-time error */
	char *s, *t;
{
	warning(s, t);
	longjmp(begin, 0);
}

fpecatch()	/* catch floating point exceptions */
{
	execerror("floating point exception", (char *) 0);
}

warning(s, t)	/* print warning message */
	char *s, *t;
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t && *t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %d\n", lineno);
}
