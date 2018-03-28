/****************************************************************************

tail.c - Source code for the tail command

AUTHOR: Gregory Pietsch <gkp1@flash.net>

DESCRIPTION:

tail prints the last part of each file on the command line (10 lines by
default); it reads from standard input if no files are given or when a
filename of `-' is encountered.  If more than one file is given, it
prints a header consisting of the file's name enclosed in `==>' and `<==' before
the output for each file.

There are two option formats for tail:  the new one, in which numbers are
arguments to the option letters; and the old one, in which the number
precedes any option letters.  In this version, the old format is barely
supported.  Supporting it fully is left as an exercise to the reader ;-).

GNU's -f (or --follow) option is not supported.  With that option, the
program loops forever on the assumption that the file being tailed is
growing.  I couldn't figure out how to determine if the program is reading
from a pipe in ANSI C; this option is ignored if reading from a pipe.

****************************************************************************/

/* include files */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* macros */

#define NO_ARG          0
#define REQUIRED_ARG    1
#define OPTIONAL_ARG    2

 /* how many characters will fill one's tail (literally) */
#define TAIL_BUFFER_SIZE 16384

 /* how much for a string buffer */
#define TAIL_STRING_BUFFER_SIZE 256

 /* need MIN */
#ifndef MIN
#define MIN(x,y) ((x)<(y)?(x):(y))
#endif

/* types */

typedef enum VERBOSITY_T {
    NEVER,
    SOMETIMES,
    ALWAYS
} VERBOSITY_T;

typedef struct LINE_QUEUE_EL_T {
    char *s;
    struct LINE_QUEUE_EL_T *next;
} LINE_QUEUE_EL_T;

typedef struct LINE_QUEUE_T {
    struct LINE_QUEUE_EL_T *first;
    struct LINE_QUEUE_EL_T *last;
    unsigned long num_elements;
} LINE_QUEUE_T;

/* GETOPT_LONG_OPTION_T: The type of long option */
typedef struct GETOPT_LONG_OPTION_T {
    char *name;                 /* the name of the long option */
    int has_arg;                /* one of the above macros */
    int *flag;                  /* determines if getopt_long() returns a
                                 * value for a long option; if it is
                                 * non-NULL, 0 is returned as a function
                                 * value and the value of val is stored in
                                 * the area pointed to by flag. Otherwise,
                                 * val is returned. */
    int val;                    /* determines the value to return if flag is
                                 * NULL. */
} GETOPT_LONG_OPTION_T;

typedef enum GETOPT_ORDERING_T {
    PERMUTE,
    RETURN_IN_ORDER,
    REQUIRE_ORDER
} GETOPT_ORDERING_T;

/* globally-defined variables */
char *optarg = NULL;
int optind = 0;
int opterr = 1;
int optopt = '?';

/* statically-defined variables */

static int show_help = 0;
static int show_version = 0;
static char *shortopts = "c:l:n:qv";
static GETOPT_LONG_OPTION_T longopts[] =
{
    {"bytes", REQUIRED_ARG, NULL, 'c'},
    {"lines", REQUIRED_ARG, NULL, 'n'},
    {"quiet", NO_ARG, NULL, 'q'},
    {"silent", NO_ARG, NULL, 'q'},
    {"verbose", NO_ARG, NULL, 'v'},

    {"help", NO_ARG, &show_help, 1},
    {"version", NO_ARG, &show_version, 1},
    {NULL, 0, 0, 0}
};
static char *program_name;
static int flag_bytes = 0;
static VERBOSITY_T flag_verbosity = SOMETIMES;
static unsigned long number = 0;
static int flag_skip = 0;

/* functions */

/* reverse_argv_elements:  reverses num elements starting at argv */
static void reverse_argv_elements(char **argv, int num)
{
    int i;
    char *tmp;

    for (i = 0; i < (num >> 1); i++) {
        tmp = argv[i];
        argv[i] = argv[num - i - 1];
        argv[num - i - 1] = tmp;
    }
}

/* permute: swap two blocks of argv-elements given their lengths */
static void permute(char **argv, int len1, int len2)
{
    reverse_argv_elements(argv, len1);
    reverse_argv_elements(argv, len1 + len2);
    reverse_argv_elements(argv, len2);
}

/* is_option: is this argv-element an option or the end of the option list? */
static int is_option(char *argv_element, int only)
{
    return ((argv_element == NULL)
            || (argv_element[0] == '-')
            || (only && argv_element[0] == '+'));
}

/* getopt_internal:  the function that does all the dirty work */
static int getopt_internal(int argc, char **argv, char *shortopts,
                 GETOPT_LONG_OPTION_T * longopts, int *longind, int only)
{
    GETOPT_ORDERING_T ordering = PERMUTE;
    static size_t optwhere = 0;
    size_t permute_from = 0;
    int num_nonopts = 0;
    int optindex = 0;
    size_t match_chars = 0;
    char *possible_arg = NULL;
    int longopt_match = -1;
    int has_arg = -1;
    char *cp;
    int arg_next = 0;

    /* first, deal with silly parameters and easy stuff */
    if (argc == 0 || argv == NULL || (shortopts == NULL && longopts == NULL))
        return (optopt = '?');
    if (optind >= argc || argv[optind] == NULL)
        return EOF;
    if (strcmp(argv[optind], "--") == 0) {
        optind++;
        return EOF;
    }
    /* if this is our first time through */
    if (optind == 0)
        optind = optwhere = 1;

    /* define ordering */
    if (shortopts != NULL && (*shortopts == '-' || *shortopts == '+')) {
        ordering = (*shortopts == '-') ? RETURN_IN_ORDER : REQUIRE_ORDER;
        shortopts++;
    }
    else
        ordering = (getenv("POSIXLY_CORRECT") != NULL) ? REQUIRE_ORDER :
            PERMUTE;

    /*
     * based on ordering, find our next option, if we're at the beginning of
     * one
     */
    if (optwhere == 1) {
        switch (ordering) {
        case PERMUTE:
            permute_from = optind;
            num_nonopts = 0;
            while (!is_option(argv[optind], only)) {
                optind++;
                num_nonopts++;
            }
            if (argv[optind] == NULL) {
                /* no more options */
                optind = permute_from;
                return EOF;
            } else if (strcmp(argv[optind], "--") == 0) {
                /* no more options, but have to get `--' out of the way */
                permute(argv + permute_from, num_nonopts, 1);
                optind = permute_from + 1;
                return EOF;
            }
            break;
        case RETURN_IN_ORDER:
            if (!is_option(argv[optind], only)) {
                optarg = argv[optind++];
                return (optopt = 1);
            }
            break;
        case REQUIRE_ORDER:
            if (!is_option(argv[optind], only))
                return EOF;
            break;
        }
    }
    /* we've got an option, so parse it */

    /* first, is it a long option? */
    if (longopts != NULL
        && (memcmp(argv[optind], "--", 2) == 0
            || (only && argv[optind][0] == '+'))
        && optwhere == 1) {
        /* handle long options */
        if (memcmp(argv[optind], "--", 2) == 0)
            optwhere = 2;
        longopt_match = -1;
        possible_arg = strchr(argv[optind] + optwhere, '=');
        if (possible_arg == NULL) {
            /* no =, so next argv might be arg */
            match_chars = strlen(argv[optind]);
            possible_arg = argv[optind] + match_chars;
            match_chars = match_chars - optwhere;
        }
        else
            match_chars = (possible_arg - argv[optind]) - optwhere;
        for (optindex = 0; longopts[optindex].name != NULL; optindex++) {
            if (memcmp(argv[optind] + optwhere,
                       longopts[optindex].name,
                       match_chars) == 0) {
                /* do we have an exact match? */
                if (match_chars == (int)(strlen(longopts[optindex].name))) {
                    longopt_match = optindex;
                    break;
                }
                /* do any characters match? */
                else {
                    if (longopt_match < 0)
                        longopt_match = optindex;
                    else {
                        /* we have ambiguous options */
                        if (opterr)
                            fprintf(stderr, "%s: option `%s' is ambiguous "
                                    "(could be `--%s' or `--%s')\n",
                                    argv[0],
                                    argv[optind],
                                    longopts[longopt_match].name,
                                    longopts[optindex].name);
                        return (optopt = '?');
                    }
                }
            }
        }
        if (longopt_match >= 0)
            has_arg = longopts[longopt_match].has_arg;
    }
    /* if we didn't find a long option, is it a short option? */
    if (longopt_match < 0 && shortopts != NULL) {
        cp = strchr(shortopts, argv[optind][optwhere]);
        if (cp == NULL) {
            /* couldn't find option in shortopts */
            if (opterr)
                fprintf(stderr,
                        "%s: invalid option -- `-%c'\n",
                        argv[0],
                        argv[optind][optwhere]);
            optwhere++;
            if (argv[optind][optwhere] == '\0') {
                optind++;
                optwhere = 1;
            }
            return (optopt = '?');
        }
        has_arg = ((cp[1] == ':')
                   ? ((cp[2] == ':') ? OPTIONAL_ARG : REQUIRED_ARG)
                   : NO_ARG);
        possible_arg = argv[optind] + optwhere + 1;
        optopt = *cp;
    }
    /* get argument and reset optwhere */
    arg_next = 0;
    switch (has_arg) {
    case OPTIONAL_ARG:
        if (*possible_arg == '=')
            possible_arg++;
        if (*possible_arg != '\0') {
            optarg = possible_arg;
            optwhere = 1;
        }
        else
            optarg = NULL;
        break;
    case REQUIRED_ARG:
        if (*possible_arg == '=')
            possible_arg++;
        if (*possible_arg != '\0') {
            optarg = possible_arg;
            optwhere = 1;
        }
        else if (optind + 1 >= argc) {
            if (opterr) {
                fprintf(stderr, "%s: argument required for option `",
                        argv[0]);
                if (longopt_match >= 0)
                    fprintf(stderr, "--%s'\n", longopts[longopt_match].name);
                else
                    fprintf(stderr, "-%c'\n", *cp);
            }
            optind++;
            return (optopt = ':');
        }
        else {
            optarg = argv[optind + 1];
            arg_next = 1;
            optwhere = 1;
        }
        break;
    case NO_ARG:
        if (longopt_match < 0) {
            optwhere++;
            if (argv[optind][optwhere] == '\0')
                optwhere = 1;
        }
        else
            optwhere = 1;
        optarg = NULL;
        break;
    }

    /* do we have to permute or otherwise modify optind? */
    if (ordering == PERMUTE && optwhere == 1 && num_nonopts != 0) {
        permute(argv + permute_from, num_nonopts, 1 + arg_next);
        optind = permute_from + 1 + arg_next;
    }
    else if (optwhere == 1)
        optind = optind + 1 + arg_next;

    /* finally return */
    if (longopt_match >= 0) {
        if (longind != NULL)
            *longind = longopt_match;
        if (longopts[longopt_match].flag != NULL) {
            *(longopts[longopt_match].flag) = longopts[longopt_match].val;
            return 0;
        }
        else
            return longopts[longopt_match].val;
    }
    else
        return optopt;
}

int getopt_long(int argc, char **argv, char *shortopts,
                 GETOPT_LONG_OPTION_T * longopts, int *longind)
{
    return getopt_internal(argc, argv, shortopts, longopts, longind, 0);
}

void help(void)
{
    puts( "OPTIONS" );
    puts( "" );
    puts( "-c N, --bytes N         Print last N bytes.  "
                                   "N is a nonzero integer," );
    puts( "                        optionally followed by one of "
                                  "the following" );
    puts( "                        characters:" );
    puts( "" );
    puts( "                        b       512-byte blocks." );
    puts( "                        k       1-kilobyte blocks." );
    puts( "                        m       1-megabyte blocks." );
    puts( "" );
    puts( "-N, -l N, -n N,         Print last N lines." );
    puts( "--lines N" );
    puts( "" );
    puts( "-q, --quiet,            Never print filename headers.  "
                                   "Normally, filename" );
    puts( "--silent                headers are printed if and only"
                                   " if more than one file" );
    puts( "                        is given on the command line." );
    puts( "" );
    puts( "-v, --verbose           Always print filename headers." );
    puts( "" );
    puts( "--help                  Print usage message and exit successfully.");
    puts( "" );
    puts( "--version               Print version"
                                " information and exit successfully." );
}

void version(void)
{
    puts( "tail - output the last part of files" );
    puts( "Version 1.0" );
    puts( "Written by Gregory Pietsch" );
}

/* allocate memory, die on error */
void *xmalloc(size_t n)
{
    void *p = malloc(n);

    if (p == NULL) {
        fprintf(stderr, "%s: out of memory\n", program_name);
        exit(EXIT_FAILURE);
    }
    return p;
}

/* reallocate memory, die on error */
void *xrealloc(void *p, size_t n)
{
    void *s;

    if (n == 0) {
        if (p != NULL)
            free(p);
        return NULL;
    }
    if (p == NULL)
        return xmalloc(n);
    s = realloc(p, n);
    if (s == NULL) {
        fprintf(stderr, "%s: out of memory\n", program_name);
        exit(EXIT_FAILURE);
    }
    return s;
}

/* get string duplicate */
char *xstrdup(char *s)
{
    char *p = xmalloc(strlen(s) + 1);

    strcpy(p, s);
    return p;
}

/* queue stuff - get fresh queue */
LINE_QUEUE_T *lq_create(void)
{
    LINE_QUEUE_T *lq = xmalloc(sizeof LINE_QUEUE_T);

    lq->first = NULL;
    lq->last = NULL;
    lq->num_elements = 0;
    return lq;
}

/* put an item onto the queue */
void lq_enq(LINE_QUEUE_T * lq, char *s)
{
    LINE_QUEUE_EL_T *lq_el = xmalloc(sizeof LINE_QUEUE_EL_T);

    lq_el->s = xstrdup(s);
    lq_el->next = NULL;
    if (lq->first == NULL && lq->last == NULL) {
        /* first element */
        lq->first = lq->last = lq_el;
        lq->num_elements = 1;
    }
    else {
        /* tack onto end */
        lq->last->next = lq_el;
        lq->last = lq_el;
        lq->num_elements++;
    }
}

/* take an item off the queue */
char *lq_deq(LINE_QUEUE_T * lq)
{
    char *s;
    LINE_QUEUE_EL_T *lq_el;

    if (lq->first == NULL)
        return NULL;
    lq_el = lq->first;
    s = lq_el->s;
    if (lq->first == lq->last)
        lq->first = lq->last = NULL;
    else
        lq->first = lq->first->next;
    free(lq_el);
    lq->num_elements--;
    return s;
}

/* output number lines -- this function is tough because I can only
 * use fseek() to rewind a text stream (See ISO C 7.9.9.2 if you don't
 * believe me).
 */
void tail_lines(FILE * f)
{
    char buffer[TAIL_BUFFER_SIZE];
    size_t num_read;
    int last_is_nl = 0;
    unsigned long num_skipped = 0;
    int c;
    LINE_QUEUE_T *lq = NULL;
    char *s;
    size_t s_size = 0;
    size_t s_allocked = 0;
    char *p;

    if (flag_skip) {
        /* skip a bunch of lines, output everything else */
        while ((c = getc(f)) != EOF && num_skipped < number) {
            if (c == '\n')
                num_skipped++;
        }
        while ((num_read = fread(buffer, 1, TAIL_BUFFER_SIZE, f)) != 0)
{
            fwrite(buffer, 1, num_read, stdout);
            last_is_nl = (buffer[num_read - 1] == '\n');
        }
        if (!last_is_nl)
            fputc('\n', stdout);
    }
    else {
        lq = lq_create();
        s = xmalloc(TAIL_STRING_BUFFER_SIZE);
        s_allocked = TAIL_STRING_BUFFER_SIZE;
        while ((c = getc(f)) != EOF) {
            /* add to s, if not at eof or end of line */
            if (c != '\n') {
                if (s_size == s_allocked - 1) {
                    s_allocked += TAIL_STRING_BUFFER_SIZE;
                    s = xrealloc(s, s_allocked);
                }
                s[s_size++] = c;
            }
            else {
                /* enqueue s, possibly dequeueing if we don't need a
line */
                s[s_size] = '\0';
                lq_enq(lq, s);
                if (lq->num_elements > number)
                    free(lq_deq(lq));
                s_size = 0;
            }
        }
        while (lq->num_elements != 0) {
            /* print out strings */
            p = lq_deq(lq);
            puts(p);
            free(p);
        }
        free(s);
        free(lq);
    }
}

/* output number characters, or skip over number characters */
void tail_chars(FILE * f)
{
    char buffer[TAIL_BUFFER_SIZE];
    size_t num_read;
    int last_is_nl = 0;
    long lnum = number;

    if (flag_skip)
        fseek(f, lnum, SEEK_SET);
    else
        fseek(f, -lnum, SEEK_END);
    while ((num_read = fread(buffer, 1, TAIL_BUFFER_SIZE, f)) != 0) {
        fwrite(buffer, 1, num_read, stdout);
        last_is_nl = (buffer[num_read - 1] == '\n');
    }
    if (!last_is_nl)
        fputc('\n', stdout);
}

void parse_args(int argc, char **argv)
{
    int opt;
    char *p;
    int flag_found_number = 0;
    int verbosity_changed = 0;

    do {
        switch ((opt = getopt_long(argc, argv, shortopts, longopts, NULL))) {
        case 'c':               /* print bytes */
            if (flag_found_number) {
                fprintf(stderr, "%s: invalid arguments\s", program_name);
                abort();
            }
            flag_bytes = 1;
            p = optarg;
            if (*p == '+') {
                flag_skip = 1;
                p++;
            }
            for (number = 0;
                 isdigit(*p);
                 number = number * 10 + (*p++ - '0'));
            switch (*p) {
            case 'b':           /* 512-byte blocks */
                number *= 512;
                break;
            case 'k':           /* kilobyte blocks */
                number *= 1024;
                break;
            case 'm':           /* megabyte blocks */
                number *= 1048576;
                break;
            default:
                break;
            }
            flag_found_number = 1;
            break;
        case 'l':
        case 'n':               /* lines */
            if (flag_found_number) {
                fprintf(stderr, "%s: invalid arguments\s", program_name);
                abort();
            }
            flag_bytes = 0;
            p = optarg;
            if (*p == '+') {
                flag_skip = 1;
                p++;
            }
            number = strtoul(p, NULL, 10);
            flag_found_number = 1;
            break;
        case 'q':               /* quiet */
            if (verbosity_changed) {
                fprintf(stderr, "%s: invalid arguments\s", program_name);
                abort();
            }
            verbosity_changed = 1;
            flag_verbosity = NEVER;
            break;
        case 'v':               /* verbose */
            if (verbosity_changed) {
                fprintf(stderr, "%s: invalid arguments\s", program_name);
                abort();
            }
            verbosity_changed = 1;
            flag_verbosity = ALWAYS;
            break;
        case '?':               /* invalid option */
            fprintf(stderr, "For help, type:\n\t%s --help\n", program_name);
            exit(EXIT_FAILURE);
        case 1:
        case 0:
            if (show_help || show_version) {
                if (show_help)
                    help();
                if (show_version)
                    version();
                exit(EXIT_SUCCESS);
            }
            break;
        default:
            break;
        }
    } while (opt != EOF);
    if (flag_found_number == 0 || number == 0) {
        /* didn't find anything, so set default */
        flag_bytes = 0;
        number = 10;
    }
}

int main(int argc, char **argv)
{
    int i;
    int j;
    unsigned long ul;
    char **new_argv = xmalloc((argc + 1) * (sizeof(char *)));
    char *allocked_argvs = xmalloc(argc + 1);
    char *p;
    char *s;
    char *t;
    FILE *f;
    int flag_plus = 0;

    memset(allocked_argvs, 0, argc + 1);
    new_argv[0] = program_name = argv[0];
    /* deal with silly old-format arguments */
    for (i = 1, j = 1; i < argc; i++) {
        p = argv[i];
        flag_plus = 0;
        /* handle options first */
        if (*p == '-' || *p == '+') {
            if (isdigit(p[1]) || p[1] == '+' || *p == '+') {
                /* rearrange p */
                s = xmalloc(strlen(p) + 3);
                t = s;
                *t++ = '-';
                if (*p == '-')
                    p++;
                ul = 0;
                if (*p == '+') {
                    flag_plus = 1;
                    p++;
                }
                while (isdigit(*p)) {
                    ul = ul * 10 + (*p - '0');
                    p++;
                }
                if (strchr(p, 'q') != NULL)
                    *t++ = 'q';
                if (strchr(p, 'v') != NULL)
                    *t++ = 'v';
                if (strpbrk(p, "cbkm") != NULL)
                    *t++ = 'c';
                if (strchr(p, 'l') != NULL)
                    *t++ = 'l';
                if (strchr(p, 'n') != NULL || t[-1] == '-')
                    *t++ = 'n';
                if (flag_plus)
                    *t++ = '+';
                sprintf(t, "%lu", ul);
                t += strlen(t);
                if (strchr(p, 'b') != NULL)
                    *t++ = 'b';
                if (strchr(p, 'k') != NULL)
                    *t++ = 'k';
                if (strchr(p, 'm') != NULL)
                    *t++ = 'm';
                *t = '\0';
                new_argv[j] = s;
                allocked_argvs[j++] = 1;
            }
            else
                new_argv[j++] = argv[i];
        }
    }
    for (i = 1; i < argc; i++) {
        /* handle file names */
        p = argv[i];
        if (*p != '-')
            new_argv[j++] = p;
    }
    new_argv[argc] = NULL;
    parse_args(argc, new_argv);
    if (optind == argc
        || (optind == argc - 1 && strcmp(argv[optind], "-") == 0)) {
        /* no more argv-elements, tail stdin */
        if (flag_verbosity == ALWAYS)
            puts("==> standard input <==");
        flag_bytes ? tail_chars(stdin) : tail_lines(stdin);
    }
    else if (optind == argc - 1) {
        /* one file */
        f = fopen(new_argv[optind], flag_bytes ? "rb" : "r");
        if (f == NULL) {
            fprintf(stderr, "%s: Can't open file %s\n",
                    program_name, new_argv[optind]);
            abort();
        }
        if (flag_verbosity == ALWAYS)
            printf("==> %s <==\n", new_argv[optind]);
        flag_bytes ? tail_chars(f) : tail_lines(f);
        fclose(f);
    }
    else {
        /* multiple files */
        for (i = optind; i < argc; i++) {
            if (strcmp(new_argv[i], "-") == 0) {
                f = stdin;
                if (flag_verbosity != NEVER)
                    puts("==> standard input <==");
            }
            else {
                f = fopen(new_argv[i], flag_bytes ? "rb" : "r");
                if (f == NULL) {
                    fprintf(stderr, "%s: can't open %s\n",
                            argv[0], argv[i]);
                    abort();
                }
                if (flag_verbosity != NEVER)
                    printf("==> %s <==\n", new_argv[i]);
            }
            flag_bytes ? tail_chars(f) : tail_lines(f);
            if (f != stdin)
                fclose(f);
        }
    }
    /* free all we can */
    for (i = 1; i <= argc; i++)
        if (allocked_argvs[i])
            free(new_argv[i]);
    free(allocked_argvs);
    return EXIT_SUCCESS;
}

/* END OF FILE tail.c */

