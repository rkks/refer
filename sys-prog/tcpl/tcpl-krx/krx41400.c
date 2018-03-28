/* EXERCISE 4-14 Gregory Pietsch */

/* conditional compilation added by RJH */

#ifdef CATEGORY_0

#define swap(t,x,y) do{t z=x;x=y;y=z}while(0)

#else
#ifdef CATEGORY_1

/*
This works if I can use the assignment operator on type t.
I didn't know if I was allowed to use sizeof or not and still remain
Level 0, otherwise this one is better:
*/

#define swap(t,x,y)                             \
do {                                            \
    (unsigned char *)a=(unsigned char *)(&(x)); \
    (unsigned char *)b=(unsigned char *)(&(y)); \
    size_t i = sizeof(t);                       \
    while (i--) {                               \
        *a ^= *b;                               \
        *b ^= *a;                               \
        *a ^= *b;                               \
        a++;                                    \
        b++;                                    \
    }                                           \
} while (0)

#endif
#endif

/* editor's note: sizeof is first mentioned on p91, after this exercise,
 * and is not explained properly until p135, so it can be used in
 * Category 0 solutions only for exercises 6-1 onward.
 */


