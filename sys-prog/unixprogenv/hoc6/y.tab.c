
#line	2	"hoc.y"
#include "hoc.h"
#define	code2(c1,c2)	code(c1); code(c2)
#define	code3(c1,c2,c3)	code(c1); code(c2); code(c3)

#line	6	"hoc.y"
typedef union  {
	Symbol	*sym;	/* symbol table pointer */
	Inst	*inst;	/* machine instruction */
	int	narg;	/* number of arguments */
} YYSTYPE;
extern	int	yyerrflag;
#ifndef	YYMAXDEPTH
#define	YYMAXDEPTH	150
#endif
YYSTYPE	yylval;
YYSTYPE	yyval;
#define	NUMBER	57346
#define	STRING	57347
#define	PRINT	57348
#define	VAR	57349
#define	BLTIN	57350
#define	UNDEF	57351
#define	WHILE	57352
#define	IF	57353
#define	ELSE	57354
#define	FUNCTION	57355
#define	PROCEDURE	57356
#define	RETURN	57357
#define	FUNC	57358
#define	PROC	57359
#define	READ	57360
#define	ARG	57361
#define	OR	57362
#define	AND	57363
#define	GT	57364
#define	GE	57365
#define	LT	57366
#define	LE	57367
#define	EQ	57368
#define	NE	57369
#define	UNARYMINUS	57370
#define	NOT	57371
#define YYEOFCODE 1
#define YYERRCODE 2

#line	115	"hoc.y"

	/* end of grammar */
#include <stdio.h>
#include <ctype.h>
char	*progname;
int	lineno = 1;
#include <signal.h>
#include <setjmp.h>
jmp_buf	begin;
int	indef;
char	*infile;	/* input file name */
FILE	*fin;		/* input file pointer */
char	**gargv;	/* global argument list */
int	gargc;

int c;	/* global for use by warning() */
yylex()		/* hoc6 */
{
	while ((c=getc(fin)) == ' ' || c == '\t')
		;
	if (c == EOF)
		return 0;
	if (c == '.' || isdigit(c)) {	/* number */
		double d;
		ungetc(c, fin);
		fscanf(fin, "%lf", &d);
		yylval.sym = install("", NUMBER, d);
		return NUMBER;
	}
	if (isalpha(c)) {
		Symbol *s;
		char sbuf[100], *p = sbuf;
		do {
			if (p >= sbuf + sizeof(sbuf) - 1) {
				*p = '\0';
				execerror("name too long", sbuf);
			}
			*p++ = c;
		} while ((c=getc(fin)) != EOF && isalnum(c));
		ungetc(c, fin);
		*p = '\0';
		if ((s=lookup(sbuf)) == 0)
			s = install(sbuf, UNDEF, 0.0);
		yylval.sym = s;
		return s->type == UNDEF ? VAR : s->type;
	}
	if (c == '$') {	/* argument? */
		int n = 0;
		while (isdigit(c=getc(fin)))
			n = 10 * n + c - '0';
		ungetc(c, fin);
		if (n == 0)
			execerror("strange $...", (char *)0);
		yylval.narg = n;
		return ARG;
	}
	if (c == '"') {	/* quoted string */
		char sbuf[100], *p, *emalloc();
		for (p = sbuf; (c=getc(fin)) != '"'; p++) {
			if (c == '\n' || c == EOF)
				execerror("missing quote", "");
			if (p >= sbuf + sizeof(sbuf) - 1) {
				*p = '\0';
				execerror("string too long", sbuf);
			}
			*p = backslash(c);
		}
		*p = 0;
		yylval.sym = (Symbol *)emalloc(strlen(sbuf)+1);
		strcpy(yylval.sym, sbuf);
		return STRING;
	}
	switch (c) {
	case '>':	return follow('=', GE, GT);
	case '<':	return follow('=', LE, LT);
	case '=':	return follow('=', EQ, '=');
	case '!':	return follow('=', NE, NOT);
	case '|':	return follow('|', OR, '|');
	case '&':	return follow('&', AND, '&');
	case '\n':	lineno++; return '\n';
	default:	return c;
	}
}

backslash(c)	/* get next char with \'s interpreted */
	int c;
{
	char *index();	/* `strchr()' in some systems */
	static char transtab[] = "b\bf\fn\nr\rt\t";
	if (c != '\\')
		return c;
	c = getc(fin);
	if (islower(c) && index(transtab, c))
		return index(transtab, c)[1];
	return c;
}

follow(expect, ifyes, ifno)	/* look ahead for >=, etc. */
{
	int c = getc(fin);

	if (c == expect)
		return ifyes;
	ungetc(c, fin);
	return ifno;
}

defnonly(s)	/* warn if illegal definition */
	char *s;
{
	if (!indef)
		execerror(s, "used outside definition");
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
	fseek(fin, 0L, 2);		/* flush rest of file */
	longjmp(begin, 0);
}

fpecatch()	/* catch floating point exceptions */
{
	execerror("floating point exception", (char *) 0);
}

main(argc, argv)	/* hoc6 */
	char *argv[];
{
	int i, fpecatch();

	progname = argv[0];
	if (argc == 1) {	/* fake an argument list */
		static char *stdinonly[] = { "-" };

		gargv = stdinonly;
		gargc = 1;
	} else {
		gargv = argv+1;
		gargc = argc-1;
	}
	init();
	while (moreinput())
		run();
	return 0;
}

moreinput()
{
	if (gargc-- <= 0)
		return 0;
	if (fin && fin != stdin)
		fclose(fin);
	infile = *gargv++;
	lineno = 1;
	if (strcmp(infile, "-") == 0) {
		fin = stdin;
		infile = 0;
	} else if ((fin=fopen(infile, "r")) == NULL) {
		fprintf(stderr, "%s: can't open %s\n", progname, infile);
		return moreinput();
	}
	return 1;
}

run()	/* execute until EOF */
{
	setjmp(begin);
	signal(SIGFPE, fpecatch);
	for (initcode(); yyparse(); initcode())
		execute(progbase);
}

warning(s, t)	/* print warning message */
	char *s, *t;
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t)
		fprintf(stderr, " %s", t);
	if (infile)
		fprintf(stderr, " in %s", infile);
	fprintf(stderr, " near line %d\n", lineno);
	while (c != '\n' && c != EOF)
		c = getc(fin);	/* flush rest of input line */
	if (c == '\n')
		lineno++;
}
short	yyexca[] =
{-1, 1,
	1, -1,
	-2, 0,
};
#define	YYNPROD	64
#define	YYPRIVATE 57344
#define	YYLAST	364
short	yyact[] =
{
  88, 104,   6, 100, 115,   5, 112, 113,  86, 113,
 111, 110, 108,  52,  99,  56,  31,  32,  33,  34,
  98,  94,  35,  65,  66,  67,  33,  34,  85,  64,
  35,  63,  68,  69,  70,  71,  72,  73,  74,  75,
  76,  77,  78,  79,  80,  59,  44,  29,  28,  27,
  35,  83,  84,  51,  50,  54,  46, 114,  95,  45,
  89,  58,  47,  48,  87,  96,  90,  93,   7,  49,
  18,  82,  14,  10,  21,  62,  25,  26,  60,  19,
  13,  12,   8,   9,  20,  11, 101, 102,  53,  81,
   4,   3, 106,   1,  16, 101,  23,  15, 107,  61,
  24,  55,   2,  22,   0,  17,   0,   0,   0,   0,
   0,   0,   0,   0, 118,   0, 116, 117,   0,   0,
 119, 120,  18,   0,  14,  10,  21,   0,  25,  26,
   0,  19,  13,  12,   0,   0,  20,  11,  36,  37,
  38,  39,  40,  41,  31,  32,  33,  34,  23,   0,
  35,   0,  24,   0,  92,  22,   0,  17,  91,  18,
   0,  14,  10,  21,   0,  25,  26,   0,  19,  13,
  12,   0,   0,  20,  11,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  23,   0,   0,   0,  24,
   0,   0,  22,   0,  17,  43,  42,  36,  37,  38,
  39,  40,  41,  31,  32,  33,  34,   0,   0,  35,
   0,   0, 109,  43,  42,  36,  37,  38,  39,  40,
  41,  31,  32,  33,  34,   0,   0,  35,   0,   0,
 105,  43,  42,  36,  37,  38,  39,  40,  41,  31,
  32,  33,  34,   0,   0,  35,   0,   0,  97,  43,
  42,  36,  37,  38,  39,  40,  41,  31,  32,  33,
  34,   0,   0,  35,  30,  43,  42,  36,  37,  38,
  39,  40,  41,  31,  32,  33,  34,  18, 103,  35,
  10,  21,   0,   0,   0,   0,  19,   0,   0,   0,
   0,  20,  11,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  23,   0,   0,   0,  24,   0,   0,
  22,  42,  36,  37,  38,  39,  40,  41,  31,  32,
  33,  34,  18,  57,  35,  10,  21,   0,   0,   0,
  18,  19,   0,  10,  21,   0,  20,  11,   0,  19,
   0,   0,   0,   0,  20,  11,   0,   0,  23,   0,
   0,   0,  24,   0,   0,  22,  23,   0,   0,   0,
  24,   0,   0,  22
};
short	yypact[] =
{
-1000,  66,-1000,  13,  12,  11, 228,  10,  49,  49,
  34,  33, 326,-1000, 318,   8,   8,-1000,-1000,-1000,
  -6,  -8, 326, 326, 326,-1000,-1000,-1000,-1000,-1000,
-1000, 326, 326, 326, 326, 326, 326, 326, 326, 326,
 326, 326, 326, 326,-1000,-1000,-1000,-1000,-1000,-1000,
 326, 326, 244,-1000,  -9, -33, 244,-1000, 155, 326,
 155, 118, -16,  51, 326, 210,  15,  15,  -5,  -5,
  15,  15,  15, -13, -13, -13, -13, -13, -13, 115,
 289, -17, -23, 244, 244, 326, 273,-1000, 244, 192,
-1000,-1000,-1000,-1000, 326, -26, 174,-1000, -27, -28,
 -32, 244, 244,-1000,-1000,-1000,  45, -34,-1000,-1000,
 155, 155,-1000, 326, 155,-1000,-1000,-1000, 244,-1000,
-1000
};
short	yypgo[] =
{
   0,   0,   5,  88, 101,  99,  61,  97,  94,  55,
   1,  59,   3,  93,  91,  89,  71
};
short	yyr1[] =
{
   0,  13,  13,  13,  13,  13,  13,  13,   3,   3,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   6,
   7,   8,   9,  10,   5,   5,   5,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   4,   4,   4,   4,  15,  14,  16,  14,  11,  11,
  11,  12,  12,  12
};
short	yyr2[] =
{
   0,   0,   2,   3,   3,   3,   3,   3,   3,   3,
   1,   1,   2,   5,   2,   4,   4,   7,   3,   3,
   1,   1,   0,   0,   0,   2,   2,   1,   1,   1,
   1,   5,   4,   4,   3,   3,   3,   3,   3,   3,
   2,   3,   3,   3,   3,   3,   3,   3,   3,   2,
   1,   1,   3,   3,   0,   6,   0,   6,   1,   1,
   1,   0,   1,   3
};
short	yychk[] =
{
-1000, -13,  36, -14,  -3,  -2,  -1,   2,  16,  17,
   7,  19,  15,  14,   6,  -7,  -8,  39,   4,  13,
  18,   8,  37,  30,  34,  10,  11,  36,  36,  36,
  36,  29,  30,  31,  32,  35,  23,  24,  25,  26,
  27,  28,  22,  21,  36, -11,   7,  13,  14, -11,
  20,  20,  -1,  -3,  -9,  -4,  -1,   5,  -6,  37,
  -6,  -5,  -9,  37,  37,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1, -15, -16,  -1,  -1,  37,  41,  -2,  -1,  -1,
  -2,  40,  36,  -2,  37,   7,  -1,  38,  37,  37,
 -12,  -1,  -1,   5, -10,  38, -10, -12,  38,  38,
  38,  38,  38,  41,  12,  38,  -2,  -2,  -1,  -2,
 -10
};
short	yydef[] =
{
   1,  -2,   2,   0,  30,   0,   0,   0,   0,   0,
  28,  29,  11,  22,   0,   0,   0,  24,  27,  22,
   0,   0,   0,   0,   0,  20,  21,   3,   4,   5,
   6,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   7,  54,  58,  59,  60,  56,
   0,   0,  12,  30,   0,  14,  50,  51,   0,   0,
   0,   0,   0,   0,   0,   0,  40,  49,  35,  36,
  37,  38,  39,  41,  42,  43,  44,  45,  46,  47,
  48,   0,   0,   8,   9,  61,   0,  23,  10,   0,
  23,  18,  25,  26,  61,   0,   0,  34,   0,   0,
   0,  62,  52,  53,  15,  19,  16,   0,  32,  33,
   0,   0,  13,   0,   0,  31,  55,  57,  63,  23,
  17
};
short	yytok1[] =
{
   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  36,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  37,  38,  31,  29,  41,  30,   0,  32,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  20,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  35,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  39,   0,  40
};
short	yytok2[] =
{
   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,
  12,  13,  14,  15,  16,  17,  18,  19,  21,  22,
  23,  24,  25,  26,  27,  28,  33,  34
};
long	yytok3[] =
{
   0
};
#define YYFLAG 		-1000
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define	yyclearin	yychar = -1
#define	yyerrok		yyerrflag = 0

#ifdef	yydebug
#include	"y.debug"
#else
#define	yydebug		0
#endif

/*	parser for yacc output	*/

int	yynerrs = 0;		/* number of errors */
int	yyerrflag = 0;		/* error recovery flag */

char*	yytoknames[1];		/* for debugging */
char*	yystates[1];		/* for debugging */
long	yychar;				/* for debugging */

extern	int	fprint(int, char*, ...);
extern	int	sprint(char*, char*, ...);

char*
yytokname(int yyc)
{
	static char x[10];

	if(yyc > 0 && yyc <= sizeof(yytoknames)/sizeof(yytoknames[0]))
	if(yytoknames[yyc-1])
		return yytoknames[yyc-1];
	sprintf(x, "<%d>", yyc);
	return x;
}

char*
yystatname(int yys)
{
	static char x[10];

	if(yys >= 0 && yys < sizeof(yystates)/sizeof(yystates[0]))
	if(yystates[yys])
		return yystates[yys];
	sprintf(x, "<%d>\n", yys);
	return x;
}

long
yylex1(void)
{
	long yychar;
	long *t3p;
	int c;

	yychar = yylex();
	if(yychar <= 0) {
		c = yytok1[0];
		goto out;
	}
	if(yychar < sizeof(yytok1)/sizeof(yytok1[0])) {
		c = yytok1[yychar];
		goto out;
	}
	if(yychar >= YYPRIVATE)
		if(yychar < YYPRIVATE+sizeof(yytok2)/sizeof(yytok2[0])) {
			c = yytok2[yychar-YYPRIVATE];
			goto out;
		}
	for(t3p=yytok3;; t3p+=2) {
		c = t3p[0];
		if(c == yychar) {
			c = t3p[1];
			goto out;
		}
		if(c == 0)
			break;
	}
	c = 0;

out:
	if(c == 0)
		c = yytok2[1];	/* unknown char */
	if(yydebug >= 3)
		printf("lex %.4X %s\n", yychar, yytokname(c));
	return c;
}

int
yyparse(void)
{
	struct
	{
		YYSTYPE	yyv;
		int	yys;
	} yys[YYMAXDEPTH], *yyp, *yypt;
	short *yyxi;
	int yyj, yym, yystate, yyn, yyg;
	YYSTYPE save1, save2;
	int save3, save4;

	save1 = yylval;
	save2 = yyval;
	save3 = yynerrs;
	save4 = yyerrflag;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyp = &yys[-1];
	goto yystack;

ret0:
	yyn = 0;
	goto ret;

ret1:
	yyn = 1;
	goto ret;

ret:
	yylval = save1;
	yyval = save2;
	yynerrs = save3;
	yyerrflag = save4;
	return yyn;

yystack:
	/* put a state and value onto the stack */
	if(yydebug >= 4)
		printf("char %s in %s", yytokname(yychar), yystatname(yystate));

	yyp++;
	if(yyp >= &yys[YYMAXDEPTH]) { 
		yyerror("yacc stack overflow"); 
		goto ret1; 
	}
	yyp->yys = yystate;
	yyp->yyv = yyval;

yynewstate:
	yyn = yypact[yystate];
	if(yyn <= YYFLAG)
		goto yydefault; /* simple state */
	if(yychar < 0)
		yychar = yylex1();
	yyn += yychar;
	if(yyn < 0 || yyn >= YYLAST)
		goto yydefault;
	yyn = yyact[yyn];
	if(yychk[yyn] == yychar) { /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if(yyerrflag > 0)
			yyerrflag--;
		goto yystack;
	}

yydefault:
	/* default state action */
	yyn = yydef[yystate];
	if(yyn == -2) {
		if(yychar < 0)
			yychar = yylex1();

		/* look through exception table */
		for(yyxi=yyexca;; yyxi+=2)
			if(yyxi[0] == -1 && yyxi[1] == yystate)
				break;
		for(yyxi += 2;; yyxi += 2) {
			yyn = yyxi[0];
			if(yyn < 0 || yyn == yychar)
				break;
		}
		yyn = yyxi[1];
		if(yyn < 0)
			goto ret0;
	}
	if(yyn == 0) {
		/* error ... attempt to resume parsing */
		switch(yyerrflag) {
		case 0:   /* brand new error */
			yyerror("syntax error");
			if(yydebug >= 1) {
				printf("%s", yystatname(yystate));
				printf("saw %s\n", yytokname(yychar));
			}
yyerrlab:
			yynerrs++;

		case 1:
		case 2: /* incompletely recovered error ... try again */
			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */
			while(yyp >= yys) {
				yyn = yypact[yyp->yys] + YYERRCODE;
				if(yyn >= 0 && yyn < YYLAST) {
					yystate = yyact[yyn];  /* simulate a shift of "error" */
					if(yychk[yystate] == YYERRCODE)
						goto yystack;
				}

				/* the current yyp has no shift onn "error", pop stack */
				if(yydebug >= 2)
					printf("error recovery pops state %d, uncovers %d\n",
						yyp->yys, (yyp-1)->yys );
				yyp--;
			}
			/* there is no state on the stack with an error shift ... abort */
			goto ret1;

		case 3:  /* no shift yet; clobber input char */
			if(yydebug >= YYEOFCODE)
				printf("error recovery discards %s\n", yytokname(yychar));
			if(yychar == 0)
				goto ret1;
			yychar = -1;
			goto yynewstate;   /* try again in the same state */
		}
	}

	/* reduction by production yyn */
	if(yydebug >= 2)
		printf("reduce %d in:\n\t%s", yyn, yystatname(yystate));

	yypt = yyp;
	yyp -= yyr2[yyn];
	yyval = (yyp+1)->yyv;
	yym = yyn;

	/* consult goto table to find next state */
	yyn = yyr1[yyn];
	yyg = yypgo[yyn];
	yyj = yyg + yyp->yys + 1;

	if(yyj >= YYLAST || yychk[yystate=yyact[yyj]] != -yyn)
		yystate = yyact[yyg];
	switch(yym) {
		
case 4:
#line	30	"hoc.y"
{ code2(pop, STOP); return 1; } break;
case 5:
#line	31	"hoc.y"
{ code(STOP); return 1; } break;
case 6:
#line	32	"hoc.y"
{ code2(print, STOP); return 1; } break;
case 7:
#line	33	"hoc.y"
{ yyerrok; } break;
case 8:
#line	35	"hoc.y"
{ code3(varpush,(Inst)yypt[-2].yyv.sym,assign); yyval.inst=yypt[-0].yyv.inst; } break;
case 9:
#line	37	"hoc.y"
{ defnonly("$"); code2(argassign,(Inst)yypt[-2].yyv.narg); yyval.inst=yypt[-0].yyv.inst;} break;
case 10:
#line	39	"hoc.y"
{ code(pop); } break;
case 11:
#line	40	"hoc.y"
{ defnonly("return"); code(procret); } break;
case 12:
#line	42	"hoc.y"
{ defnonly("return"); yyval.inst=yypt[-0].yyv.inst; code(funcret); } break;
case 13:
#line	44	"hoc.y"
{ yyval.inst = yypt[-3].yyv.inst; code3(call, (Inst)yypt[-4].yyv.sym, (Inst)yypt[-1].yyv.narg); } break;
case 14:
#line	45	"hoc.y"
{ yyval.inst = yypt[-0].yyv.inst; } break;
case 15:
#line	46	"hoc.y"
{
		(yypt[-3].yyv.inst)[1] = (Inst)yypt[-1].yyv.inst;	/* body of loop */
		(yypt[-3].yyv.inst)[2] = (Inst)yypt[-0].yyv.inst; } break;
case 16:
#line	49	"hoc.y"
{	/* else-less if */
		(yypt[-3].yyv.inst)[1] = (Inst)yypt[-1].yyv.inst;	/* thenpart */
		(yypt[-3].yyv.inst)[3] = (Inst)yypt[-0].yyv.inst; } break;
case 17:
#line	52	"hoc.y"
{	/* if with else */
		(yypt[-6].yyv.inst)[1] = (Inst)yypt[-4].yyv.inst;	/* thenpart */
		(yypt[-6].yyv.inst)[2] = (Inst)yypt[-1].yyv.inst;	/* elsepart */
		(yypt[-6].yyv.inst)[3] = (Inst)yypt[-0].yyv.inst; } break;
case 18:
#line	56	"hoc.y"
{ yyval.inst = yypt[-1].yyv.inst; } break;
case 19:
#line	58	"hoc.y"
{ code(STOP); yyval.inst = yypt[-1].yyv.inst; } break;
case 20:
#line	60	"hoc.y"
{ yyval.inst = code3(whilecode,STOP,STOP); } break;
case 21:
#line	62	"hoc.y"
{ yyval.inst = code(ifcode); code3(STOP,STOP,STOP); } break;
case 22:
#line	64	"hoc.y"
{ yyval.inst = progp; } break;
case 23:
#line	66	"hoc.y"
{ code(STOP); yyval.inst = progp; } break;
case 24:
#line	68	"hoc.y"
{ yyval.inst = progp; } break;
case 27:
#line	72	"hoc.y"
{ yyval.inst = code2(constpush, (Inst)yypt[-0].yyv.sym); } break;
case 28:
#line	73	"hoc.y"
{ yyval.inst = code3(varpush, (Inst)yypt[-0].yyv.sym, eval); } break;
case 29:
#line	74	"hoc.y"
{ defnonly("$"); yyval.inst = code2(arg, (Inst)yypt[-0].yyv.narg); } break;
case 31:
#line	77	"hoc.y"
{ yyval.inst = yypt[-3].yyv.inst; code3(call,(Inst)yypt[-4].yyv.sym,(Inst)yypt[-1].yyv.narg); } break;
case 32:
#line	78	"hoc.y"
{ yyval.inst = code2(varread, (Inst)yypt[-1].yyv.sym); } break;
case 33:
#line	79	"hoc.y"
{ yyval.inst=yypt[-1].yyv.inst; code2(bltin, (Inst)yypt[-3].yyv.sym->u.ptr); } break;
case 34:
#line	80	"hoc.y"
{ yyval.inst = yypt[-1].yyv.inst; } break;
case 35:
#line	81	"hoc.y"
{ code(add); } break;
case 36:
#line	82	"hoc.y"
{ code(sub); } break;
case 37:
#line	83	"hoc.y"
{ code(mul); } break;
case 38:
#line	84	"hoc.y"
{ code(div); } break;
case 39:
#line	85	"hoc.y"
{ code (power); } break;
case 40:
#line	86	"hoc.y"
{ yyval.inst=yypt[-0].yyv.inst; code(negate); } break;
case 41:
#line	87	"hoc.y"
{ code(gt); } break;
case 42:
#line	88	"hoc.y"
{ code(ge); } break;
case 43:
#line	89	"hoc.y"
{ code(lt); } break;
case 44:
#line	90	"hoc.y"
{ code(le); } break;
case 45:
#line	91	"hoc.y"
{ code(eq); } break;
case 46:
#line	92	"hoc.y"
{ code(ne); } break;
case 47:
#line	93	"hoc.y"
{ code(and); } break;
case 48:
#line	94	"hoc.y"
{ code(or); } break;
case 49:
#line	95	"hoc.y"
{ yyval.inst = yypt[-0].yyv.inst; code(not); } break;
case 50:
#line	97	"hoc.y"
{ code(prexpr); } break;
case 51:
#line	98	"hoc.y"
{ yyval.inst = code2(prstr, (Inst)yypt[-0].yyv.sym); } break;
case 52:
#line	99	"hoc.y"
{ code(prexpr); } break;
case 53:
#line	100	"hoc.y"
{ code2(prstr, (Inst)yypt[-0].yyv.sym); } break;
case 54:
#line	102	"hoc.y"
{ yypt[-0].yyv.sym->type=FUNCTION; indef=1; } break;
case 55:
#line	103	"hoc.y"
{ code(procret); define(yypt[-4].yyv.sym); indef=0; } break;
case 56:
#line	104	"hoc.y"
{ yypt[-0].yyv.sym->type=PROCEDURE; indef=1; } break;
case 57:
#line	105	"hoc.y"
{ code(procret); define(yypt[-4].yyv.sym); indef=0; } break;
case 61:
#line	111	"hoc.y"
{ yyval.narg = 0; } break;
case 62:
#line	112	"hoc.y"
{ yyval.narg = 1; } break;
case 63:
#line	113	"hoc.y"
{ yyval.narg = yypt[-2].yyv.narg + 1; } break;
	}
	goto yystack;  /* stack new state and value */
}
