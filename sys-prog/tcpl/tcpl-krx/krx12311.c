/* torek@elf.bsdi.com (Chris Torek) */

/*
"Write a program to remove all comments from a C program. Don't forget
to handle quoted strings and character constants properly. C comments do
not nest."

Well, what the heck.  I mailed this a day or two ago, but here is
the posted version.  I modified the problem a bit: it removes
comments from full ANSI C89 or C99 programs, handling trigraphs
and \-newline sequences.  It attempts to preserve any trigraphs in
the output, even while examining them in the "C code" as their
translated characters.  (I am not sure why I bothered doing all of
them, when only ??/ matters here.)  It keeps output line numbers in
sync with input line numbers, so that if the output is compiled,
any error messages will refer back to the proper input source line.

Lightly tested.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This flag controls whether we do trigraph processing.
 */
int	trigraphs = 1;

/*
 * This flag controls whether a comment becomes "whitespace" (ANSI C)
 * or "nothing at all" (some pre-ANSI K&R C compilers).
 */
int	whitespace = 1;

/*
 * This flag controls whether we do C89 or C99.  (C99 also handles C++.)
 */
int	c99;

/*
 * These are global so that options() can get at them, and for later
 * error messages if needed.
 */
const char *inname, *outname;

int options(const char *, char **);
void usage(void);

void	process(FILE *, FILE *);

#ifdef __GNUC__
void	panic(const char *) __attribute__((noreturn));
#else
void	panic(const char *);
#endif

int main(int argc, char **argv) {
	int i;
	FILE *in, *out;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-')
			i += options(argv[i] + 1, argv + i + 1);
		else if (inname == NULL)
			inname = argv[i];
		else
			usage();
	}
	if (inname != NULL) {
		if ((in = fopen(inname, "r")) == NULL) {
			fprintf(stderr, "cannot open %s for reading\n", inname);
			exit(EXIT_FAILURE);
		}
	} else {
		inname = "stdin";
		in = stdin;
	}
	if (outname != NULL) {
		if ((out = fopen(outname, "w")) == NULL) {
			fprintf(stderr, "cannot open %s for writing\n",
			    outname);
			exit(EXIT_FAILURE);
		}
	} else {
		outname = "stdout";
		out = stdout;
	}
	process(in, out);
	fclose(in);
	fclose(out);
	exit(EXIT_SUCCESS);
}

/*
 * This scans for -o type options.  Options that have an argument
 * can either take it immediately or as a subsequent argument (e.g.,
 * -ofoo means the same thing as -o foo).  We return 0 for "handled
 * them normally", 1 for "handled them normally but needed more
 * arguments".
 *
 * Currently this function is more powerful than really needed, but
 * if we ever decide to have more arguments...
 */
int options(const char *afterdash, char **moreargs) {
	int nmore = 0, c;

	while ((c = *afterdash++) != '\0') {
		if (c == 'o') {
			if (*afterdash) {
				outname = afterdash;
				afterdash = "";
			} else if (moreargs[nmore] != NULL)
				outname = moreargs[nmore++];
			else
				usage();
		} else if (c == 't')
			trigraphs = 0;
		else if (c == 'w')
			whitespace = 0;
		else if (c == '9')
			c99 = 1;
		else
			usage();
	}
	return nmore;
}

void usage(void) {
	fprintf(stderr, "usage: uncomment [-9tw] [-o outfile] [infile]\n");
	exit(EXIT_FAILURE);	/* ??? */
}

/*
 * States, level 0:
 *	normal
 *	trigraph processing: Q1 Q2 (for ??x)
 *
 * States, level 1:
 *	backslash-newline processing: BACK (seen \, may consume NL)
 *
 * States, level 2:
 *	normal
 *	character constant: CC (seen '), CCBACK (seen \ inside CC)
 *	string constant: SC, SCBACK
 *	comment: SLASH, COMM, COMMSTAR (for /, in-comment, & seen-star)
 *	C99: SLASHSLASH
 */

enum l0state {
	L0_NORMAL,
	L0_Q1, L0_Q2
};
enum l1state {
	L1_NORMAL,
	L1_BACK
};
enum l2state {
	L2_NORMAL,
	L2_CC, L2_CCBACK,
	L2_SC, L2_SCBACK,
	L2_SLASH, L2_COMM, L2_COMMSTAR,
	L2_SLASHSLASH
};

struct state {
	FILE *in;
	enum l0state l0state;
	int npushback;
	char pushback[4];
	char pushorig[4];	/* nonzero => trigraph pushback */
	int lastgetc;
	int lineno;
};

/*
 * Set up "initial" state.
 */
static void state0(struct state *sp, FILE *in) {
	sp->in = in;
	sp->l0state = L0_NORMAL;
	sp->npushback = 0;
	sp->lastgetc = 0;
	sp->lineno = 1;
}

static void pushback(struct state *sp, int c, char origc) {
	assert(sp->npushback < sizeof sp->pushback);
	sp->pushback[sp->npushback] = c;
	sp->pushorig[sp->npushback++] = origc;
}

/*
 * Get a character, doing trigraph processing.  Set *origc to 0 for normal
 * characters, or the actual input character pre-trigraph-mapping
 * for trigraph input.
 *
 * As a side effect, this can wind up getting up to 3 characters, maybe
 * stuffing two of them into the pushback buffer sp->buf[].  It also bumps
 * sp->lineno when a previously-read newline has been passed over.
 */
static int getl0char(struct state *sp, char *origc) {
	int c, newc;
	enum l0state state;

	state = sp->l0state;
	*origc = 0;
	while ((c = getc(sp->in)) != EOF) {
		if (sp->lastgetc == '\n')
			sp->lineno++;
		sp->lastgetc = c;
		switch (state) {

		case L0_NORMAL:
			/* ? => get another character; otherwise we are ok */
			if (c == '?') {
				state = L0_Q1;
				continue;
			}
			assert(sp->l0state == L0_NORMAL);
			return c;

		case L0_Q1:
			/* ?? => get another character */
			if (c == '?') {
				state = L0_Q2;
				continue;
			}
			/* ?X => return ?, look at X later */
			pushback(sp, c, 0);
			sp->l0state = L0_NORMAL;
			return '?';

		case L0_Q2:
			/*
			 * ??X, where X is trigraph => map
			 * ??X, where X is non-trigraph => tricky
			 * ??? => also tricky
			 */
			switch (c) {
			case '=':
				newc = '#';
				break;
			case '(':
				newc = '[';
				break;
			case '/':
				newc = '\\';
				break;
			case ')':
				newc = ']';
				break;
			case '\'':
				newc = '^';
				break;
			case '<':
				newc = '{';
				break;
			case '!':
				newc = '|';
				break;
			case '>':
				newc = '}';
				break;
			case '?':
				/*
				 * This one is slightly tricky.  Three '?'s
				 * mean that the '?' we read two characters
				 * ago gets returned, and the two remaining
				 * '?'s leave us in Q2 state.
				 */
				sp->l0state = L0_Q2;
				return '?';
			default:
				/*
				 * This one returns the first ?, leaves
				 * the second ? to be re-examined, and
				 * leaves the last character to be re-examined.
				 * In any case we are back in "normal" state.
				 */
				pushback(sp, c, 0);
				pushback(sp, '?', 0);
				sp->l0state = L0_NORMAL;
				return '?';
			}
			/* mapped a trigraph char -- return new char */
			*origc = c;
			sp->l0state = L0_NORMAL;
			return newc;

		default:
			panic("getl0char state");
		}
	}
	sp->lastgetc = EOF;
	return EOF;
}

void warn(struct state *, const char *);

void process(FILE *in, FILE *out) {
	enum l1state l1state = L1_NORMAL;
	enum l2state l2state = L2_NORMAL;
	int c, pendnls;
	char origc, backc;
	struct state state;

	state0(&state, in);
	pendnls = 0;
	backc = 0;		/* defeat gcc warning */

	/*
	 * Slight sort-of-bug: files ending in \ cause two "final" getc()s.
	 */
	do {
		if (state.npushback) {
			c = state.pushback[--state.npushback];
			origc = state.pushorig[state.npushback];
		} else if (trigraphs) {
			c = getl0char(&state, &origc);
		} else {
			c = getc(in);
			origc = 0;
			if (state.lastgetc == '\n')
				state.lineno++;
			state.lastgetc = c;
		}

		/*
		 * Do backslash-newline processing.
		 */
		switch (l1state) {

		case L1_NORMAL:
			if (c == '\\') {
				l1state = L1_BACK;
				backc = origc;
				continue;
			}
			break;

		case L1_BACK:
			/*
			 * If backc is nonzero here, the backslash that
			 * got us into this state was spelled ??/ --
			 * if we eat a newline (and hence the backslash),
			 * we forget that the eaten newline was spelled
			 * this way.  This is sort of a bug, but so it goes.
			 */
			l1state = L1_NORMAL;
			if (c == '\n') {
				pendnls++;
				continue;
			}
			if (c != EOF)
				pushback(&state, c, origc);
			c = '\\';
			origc = backc;
			break;

		default:
			panic("bad l1state");
		}

		/*
		 * Now ready to do "C proper" processing.
		 */
#define	SYNCLINES()	while (pendnls) putc('\n', out), pendnls--
#define	OUTPUT(ch, tri) ((tri) ? fprintf(out, "??%c", tri) : putc(ch, out))
#define	COPY()		OUTPUT(c, origc)

		switch (l2state) {
		case L2_NORMAL:
			switch (c) {
			case '\'':
				l2state = L2_CC;
				break;
			case '"':
				l2state = L2_SC;
				break;
			case '/':
				l2state = L2_SLASH;
				continue;
			default:
				break;
			}
			SYNCLINES();
			if (c != EOF)
				COPY();
			break;

		case L2_CC:
			switch (c) {
			case EOF:
				warn(&state, "EOF in character constant");
				break;
			case '\n':
				warn(&state, "newline in character constant");
				break;
			case '\\':
				l2state = L2_CCBACK;
				break;
			case '\'':
				l2state = L2_NORMAL;
				break;
			default:
				break;
			}
			if (c != EOF)
				COPY();
			break;

		case L2_CCBACK:
			switch (c) {
			case EOF:
				warn(&state, "EOF in character constant");
				break;
			case '\n':
				warn(&state, "newline in character constant");
				break;
			default:
				break;
			}
			l2state = L2_CC;
			if (c != EOF)
				COPY();
			break;

		case L2_SC:	/* much like CC */
			switch (c) {
			case EOF:
				warn(&state, "EOF in string constant");
				break;
			case '\n':
				warn(&state, "newline in string constant");
				break;
			case '\\':
				l2state = L2_SCBACK;
				break;
			case '"':
				l2state = L2_NORMAL;
				break;
			default:
				break;
			}
			if (c != EOF)
				COPY();
			break;

		case L2_SCBACK:
			switch (c) {
			case EOF:
				warn(&state, "EOF in string constant");
				break;
			case '\n':
				warn(&state, "newline in string constant");
				break;
			default:
				break;
			}
			l2state = L2_SC;
			if (c != EOF)
				COPY();
			break;

		case L2_SLASH:
			if (c == '*')
				l2state = L2_COMM;
			else if (c99 && c == '/')
				l2state = L2_SLASHSLASH;
			else {
				SYNCLINES();
				OUTPUT('/', 0);
				if (c != '/') {
					if (c != EOF)
						COPY();
					l2state = L2_NORMAL;
				}
			}
			break;

		case L2_COMM:
			switch (c) {
			case '*':
				l2state = L2_COMMSTAR;
				break;
			case '\n':
				pendnls++;
				break;
			case EOF:
				warn(&state, "EOF inside comment");
				break;
			}
			break;

		case L2_COMMSTAR:
			switch (c) {
			case '/':
				l2state = L2_NORMAL;
				/*
				 * If comments become whitespace,
				 * and we have no pending newlines,
				 * must emit a blank here.
				 *
				 * The comment text is now all eaten.
				 */
				if (whitespace && pendnls == 0)
					putc(' ', out);
				SYNCLINES();
				break;
			case '*':
				/* stay in L2_COMMSTAR state */
				break;
			case EOF:
				warn(&state, "EOF inside comment");
				break;
			case '\n':
				pendnls++;
				/* FALLTHROUGH */
			default:
				l2state = L2_COMM;
			}
			break;

		case L2_SLASHSLASH:
			switch (c) {
			case EOF:
				/* ??? do we really care? */
				warn(&state, "EOF inside //-comment");
				break;
			case '\n':
				l2state = L2_NORMAL;
				pendnls++;	/* cheesy, but... */
				SYNCLINES();
			default:
				break;
			}
			break;

		default:
			panic("bad l2state");
		}
	} while (c != EOF);
	SYNCLINES();
}

void warn(struct state *sp, const char *msg) {
	fprintf(stderr, "uncomment: %s(%d): %s\n", inname, sp->lineno, msg);
}

void panic(const char *msg) {
	fprintf(stderr, "panic: %s\n", msg);
	abort();
	exit(EXIT_FAILURE);
}
