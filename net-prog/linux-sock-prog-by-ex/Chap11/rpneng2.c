/* rpneng2.c :
 *
 * RPN Engine :
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <gmp.h>

typedef void (*mpz_func)(mpz_t,
    const mpz_t,const mpz_t);
typedef void (*mpz_unary)(mpz_t,const mpz_t);
typedef int (*rpn_spec)(void);

/*
 * RPN Stack :
 */
#define MAX_STACK       32
mpz_t **stack;
int sp = 0;

/*
 * Allocate a new mpz_t value :
 */
static mpz_t *
rpn_alloc(void) {
    mpz_t *v = malloc(sizeof(mpz_t));
    mpz_init(*v);
    return v;
}

/*
 * Duplicate a mpz_t value :
 */
static mpz_t *
rpn_duplicate(mpz_t *value) {
    mpz_t *v = rpn_alloc();

    mpz_set(*v,*value);
    return v;
}

/*
 * Free an allocated mpz_t value :
 */
void
rpn_free(mpz_t **v) {
    mpz_clear(**v);
    free(*v);
    *v = NULL;
}

/*
 * Push an mpz_t value onto the stack :
 */
static int
rpn_push(mpz_t *value) {
    if ( sp >= MAX_STACK )
        return -1;
    stack[sp] = value;
    return sp++;
}

/*
 * Pop a mpz_t value from the stack :
 */
static int
rpn_pop(mpz_t **value) {
    if ( sp <= 0 )
        return -1;
    *value = stack[--sp];
    return sp;
}

/*
 * Duplicate the top value on the stack :
 */
static int
rpn_dup(void) {
    mpz_t *opr2;

    if ( sp <= 0 )
        return -1;

    opr2 = rpn_alloc();
    mpz_set(*opr2,*stack[sp-1]);
    return rpn_push(opr2);
}

/*
 * Swap the top two values on the stack :
 */
static int
rpn_swap(void) {
    mpz_t *opr1, *opr2;

    if ( sp < 2 )
        return -1;

    rpn_pop(&opr1);
    rpn_pop(&opr2);
    rpn_push(opr1);
    return rpn_push(opr2);
}

/*
 * Dump the stack :
 */
static void
rpn_dump(FILE *tx) {
    int sx;

    for ( sx=sp-1; sx >= 0; --sx ) {
        fprintf(tx,"%d:",sx);
        mpz_out_str(tx,10,*stack[sx]);
        fputc('\n',tx);
    }
    fputs("E:end of stack dump\n",tx);
}

/*
 * Operation "seed" :
 *
 * OPERANDS:
 *  1:  the least significant 32 bits
 *      will seed the random number
 *      generator via srand(3) :
 * RESULTS:
 *  none.
 */
static int
rpn_seed(void) {
    int z;
    mpz_t *opr;
    long lv;

    if ( (z = rpn_pop(&opr)) < 0 )
        /* No operand available */
        return -1;

    /*
     * Get long value, ignoring errors.
     * Then seed the random number
     * generator:
     */
    lv = mpz_get_si(*opr);
    srand((int)lv);

    rpn_free(&opr);
    return z;
}

/*
 * Operation "random" :
 *
 * OPERANDS:
 *  1.  A modulo value to apply after
 *      the random number is generated.
 * RESULTS:
 *  1.  A random value: 0 < modulo value.
 */
static int
rpn_random(void) {
    mpz_t *opr, *res;
    mpz_t r;
    size_t limbs;

    if ( rpn_pop(&opr) < 0 )
        /* No operand available */
        return -1;

    mpz_init(r);
    res = rpn_alloc();

    /*
     * Pop the top to use as the modulo
     * operand. Generate a random number
     * r. Then compute r % opr as the
     * final result :
     */
    limbs = mpz_size(*opr);
    mpz_random(r,limbs);
    mpz_tdiv_r(*res,r,*opr);

    mpz_clear(r);
    rpn_free(&opr);

    return rpn_push(res);
}

/*
 * Operation "tprime" :
 *
 * Test for probability of being
 * a prime number:
 *
 * OPERANDS:
 *  1.  Number to test
 *  2.  Number of tests to try
 *      (typically 25)
 * RESULTS:
 *  1.  Number tested is probably
 *      prime when value = 1.
 *      Number tested is not prime
 *      when result is zero.
 */
static int
rpn_test_prime(void) {
    mpz_t *opr1, *opr2;
    long reps;
    int z;

    if ( sp < 2 )
        /* Insufficient operands */
        return -1;

    rpn_pop(&opr1);
    rpn_pop(&opr2);

    if ( mpz_size(*opr2) > 1 )
        /* Too many limbs in size */
        return -1;

    reps = mpz_get_si(*opr2);
    if ( reps < 1L || reps > 32768L )
        /* Too large for opr2 */
        return -1;
    
    z = mpz_probab_prime_p(*opr1,reps);
    mpz_set_si(*opr1,(long)z);
    rpn_free(&opr2);

    return rpn_push(opr1);
}

/*
 * Operation "genprime" :
 *
 * Generate a random prime number.
 *
 * OPERANDS:
 *  1.  The modulo value to apply
 *      to the randomizing value
 *      (see "random".)
 *  2.  The number of primality
 *      tests to perform (typically
 *      this value is 25.)
 * RESULTS:
 *  1.  The randomly generated prime
 *      number (actually, only a high
 *      probability of being prime.)
 */
static int
rpn_genprime(void) {
    mpz_t *opr1;
    mpz_t *opr2;
    mpz_t *res;

    if ( sp < 2 )
        return -1;

    rpn_pop(&opr1);
    rpn_pop(&opr2);

    for (;;) {
        rpn_push(rpn_duplicate(opr1));
        rpn_random();
        rpn_dup();
        rpn_push(rpn_duplicate(opr2));
        rpn_swap();
        rpn_test_prime();
        rpn_pop(&res);
        if ( mpz_cmp_si(*res,0L) != 0 )
            break;
        rpn_free(&res);
        rpn_pop(&res);
        rpn_free(&res);
    }

    rpn_free(&res);
    rpn_free(&opr2);
    rpn_free(&opr1);

    return sp - 1;
}

/*
 * Standard binary arithmetic operations:
 *
 * OPERANDS:
 *  1.  Operand 2
 *  2.  Operand 1
 *
 * RESULTS:
 *  1.  Operand 1 op Operand 2
 */
static int
rpn_binoper(mpz_func f) {
    mpz_t *res, *opr1, *opr2;

    if ( sp < 2 )
        /* Insufficient operands */
        return -1;

    res = rpn_alloc();
    rpn_pop(&opr2);
    rpn_pop(&opr1);

    f(*res,*opr1,*opr2);

    rpn_free(&opr1);
    rpn_free(&opr2);
    return rpn_push(res);
}

/*
 * Standard Unary Operations :
 *
 * OPERANDS:
 *  1.  Operand 1
 *
 * RESULTS:
 *  1.  Result of unary operation.
 */
static int
rpn_unaryop(mpz_unary f) {
    mpz_t *res, *opr1;

    if ( sp < 1 )
        /* Insufficient operands */
        return -1;

    res = rpn_alloc();
    rpn_pop(&opr1);

    f(*res,*opr1);

    rpn_free(&opr1);
    return rpn_push(res);
}

/*
 * Execute RPN operation :
 *
 * RETURNS:
 *  0   Successful.
 *  -1  Failed.
 */
static int
rpn_opr(char *oper) {
    int x;
    static struct {
        char    *oper;
        rpn_spec func;
    } spec[] = {
        { "dup", rpn_dup },
        { "swap", rpn_swap },
        { "seed", rpn_seed },
        { "random", rpn_random },
        { "tprime", rpn_test_prime },
        { "genprime", rpn_genprime },
        { 0 }
    };
    static struct {
        char    *oper;
        mpz_func func;
    } binops[] = {
        { "+", mpz_add },
        { "-", mpz_sub },
        { "*", mpz_mul },
        { "/", mpz_tdiv_q },
        { "%", mpz_tdiv_r },
        { "gcd", mpz_gcd },
        { 0 }
    };
    static struct {
        char    *oper;
        mpz_unary func;
    } unary[] = {
        { "abs", mpz_abs },
        { "neg", mpz_neg },
        { "sqrt", mpz_sqrt },
        { 0 }
    };

    /*
     * Special Cases:
     */
    for ( x=0; spec[x].oper; ++x )
        if ( !strcmp(spec[x].oper,oper) )
            return spec[x].func();

    /*
     * Test for a match on binary operators :
     */
    for ( x=0; binops[x].oper; ++x )
        if ( !strcmp(binops[x].oper,oper) )
            return rpn_binoper(binops[x].func);

    /*
     * Test for a match on unary operators :
     */
    for ( x=0; unary[x].oper; ++x )
        if ( !strcmp(unary[x].oper,oper) )
            return rpn_unaryop(unary[x].func);

    return -1;  /* Failed: unknown operator */
}

void
rpn_process(FILE *tx,char *buf) {
    int z;
    mpz_t *t;
    char *operation;
    char *operand;

    operation=strtok(buf,":\n\r");
    operand=strtok(NULL,"\n\r");

    if ( !strcmp(operation,"dump") ) {
        rpn_dump(tx);

    } else if ( !strcmp(operation,"=") ) {
        /*
         * Pop off the result :
         */
        if ( (z = rpn_pop(&t)) == -1 )
            fputs("E:Nothing to pop\n",tx);
        else {
            fprintf(tx,"%d:",z);
            mpz_out_str(tx,10,*t);
            fputc('\n',tx);
            rpn_free(&t);
        }

    } else if ( !strcmp(operation,"#") ) {
        /*
         * Push an operand onto the stack :
         */
        t = rpn_alloc();
        if ( !mpz_set_str(*t,operand,10) )
            fprintf(tx,"%d:\n",rpn_push(t));
        else {
            fputs("E:Invalid number\n",tx);
            rpn_free(&t);
        }

    } else {
        /*
         * Perform an operation :
         */
        z = rpn_opr(operation);
        if ( z == -1 )
            fprintf(tx,
                "E:Operation failed.\n");
        else
            fprintf(tx,"%d:\n",z);
    }

    fflush(tx);
}
