%{
#include "hoc.h"
#include "y.tab.h"
extern int lineno;
%}
%%
[ \t]	{ ; }	/* skip blanks and tabs */
[0-9]+\.?|[0-9]*\.[0-9]+ {
	sscanf(yytext, "%lf", &yylval.val); return NUMBER; }
[a-zA-Z][a-zA-Z0-9]* {
	Symbol *s;
	if ((s=lookup(yytext)) == 0)
		s = install(yytext, UNDEF, 0.0);
	yylval.sym = s;
	return s->type == UNDEF ? VAR : s->type; }
\n	{ lineno++; return '\n'; }   /* everything else */
.	{ return yytext[0]; }
