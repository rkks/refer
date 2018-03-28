
typedef union  {
	Symbol	*sym;	/* symbol table pointer */
	Inst	*inst;	/* machine instruction */
	int	narg;	/* number of arguments */
}	YYSTYPE;
extern	YYSTYPE	yylval;
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
