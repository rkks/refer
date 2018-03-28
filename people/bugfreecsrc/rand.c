/*pm--------------------------------------------------------------
 *
 *  OUTLINE:
 *
 *    This module implements a random number object.
 *
 *  IMPLEMENTATION:
 *
 *    The random numbers are implemented just like they are
 *    in the Microsoft C8 RTL.
 *
 *  NOTES:
 *
 *--------------------------------------------------------------*/

#define USE_HRAND

#include "app.h"

USEWINASSERT

/*--- The random number class object ---*/
CLASS(hRand, HRAND) {
    long lRand;
    };

/*--- Implement random numbers just like the MS C8 RTL ---*/
#define NEXTRAND(l)  (l*214013L+2531011L)
#define FINALRAND(l) ((l>>16)&0x7FFF)


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Create Random Number Object)  JLJ
 *
 *    Given a seed, create a new random number generator object
 *
 *  ARGUMENTS:
 *
 *    nSeed - Seed of the new random number generator
 *
 *  RETURNS:
 *
 *    A new random number generator object
 *
 *--------------------------------------------------------------*/

HRAND APIENTRY RandCreate( int nSeed )
{
    HRAND hRand;
    NEWOBJ(hRand);
    hRand->lRand = nSeed;
    return (hRand);

} /* RandCreate */


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Destroy Random Number Object)  JLJ
 *
 *    Destroy the given random number generator object
 *
 *  ARGUMENTS:
 *
 *    hRand - Random number generator object or NULL
 *
 *  RETURNS:
 *
 *    NULL
 *
 *--------------------------------------------------------------*/

HRAND APIENTRY RandDestroy( HRAND hRand )
{
    VERIFYZ(hRand) {
        FREE(hRand);
        }
    return (NULL);

} /* RandDestroy */


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Generate Next Random Number)  JLJ
 *
 *    Generate the next random number for the given random
 *    number generator object.
 *
 *  ARGUMENTS:
 *
 *    hRand - Random number generator object
 *
 *  RETURNS:
 *
 *    The next random number
 *
 *--------------------------------------------------------------*/

int APIENTRY RandNext( HRAND hRand )
{
    int nRand=0;
    VERIFY(hRand) {
        hRand->lRand = NEXTRAND(hRand->lRand);
        nRand = (int)FINALRAND(hRand->lRand);
        }
    return(nRand);

} /* RandNext */
