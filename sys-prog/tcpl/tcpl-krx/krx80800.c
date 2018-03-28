/* bfree(): Exercise from K&R 8-8; Ads an arbitrary block into the free *
** list maintained by malloc() and free() as written by K&R in chapter 8*
** Messages the block to a format acceptable to the free() list,with the*
** remaining bits (bytes) managed by Waiting To Be free()ed (wtbfree()) *
** wtbfree() can be an empty function with the effect of simply         *
** discarding the chopped off bits. Only intended to be as portable as  *
** free() itself, or maybe less so due to the use of the ALIGN macro.  */
#define ALIGN(p) (sizeof(Align)-((unsigned)(p)%sizeof(Align)))%sizeof(Align)
/* hopelessly unportable, as p is a pointer */

void wtbfree(void *p, unsigned n);

void bfree(void *p, unsigned n)
{

    unsigned align, s, r;

    if(n < sizeof(Header)) {        /* can't free less than this */
        wtbfree(p, n);               /* put in WTBfree list       */
        return;
    }
    align = ALIGN(p);
    if(align) {                     /* adjust alignment              */
        wtbfree(p, align);          /* put beginning in WTBfree list */
        p = (char *)p + align, n -= align;
    }
    s = n / sizeof(Header), r = n % sizeof(Header);
    if(r)                         /* put trailing end in WTBfree list   */
        wtbfree((char *)p+n-r, r); 
    if(s) {                       /* if there is something left to free */
        if (freep == NULL) {      /* Set up free list if it's empty     */
            base.s.ptr = freep = &base;
            base.s.size = 0;
        }
        ((Header *)p)->s.size = s;
        free((Header *)p + 1);
    }
}

struct wtbheader {
    struct wtbheader *next;
    void *p;
    unsigned n;
};

void try_to_myfree(struct wtbheader *p)
{
   char *tp; unsigned align; unsigned n;

   tp = p->p, align = ALIGN(p->p);
   if(align < p->n &&
   (p->n - align) % sizeof(Header) == 0) {
       tp += align, n = p->n - align, p->n = align;
       ((Header *)tp)->s.size = n / sizeof(Header);
       free(((Header *)tp) +1);
   }
}

static struct wtbheader *headptr;

void wtbfree(void *p, unsigned n)
{
    struct wtbheader *hp, *prevp;

    if(headptr == NULL) {                         /* first use */
        if(! (headptr = malloc(sizeof *headptr))) /* can't save fragment, dump it */
            return;
        headptr->p = p, headptr->n = n, headptr->next = NULL;
    }

    else if(p < headptr->p) {               /* Special case: less than head */
        if ((char *)p + n == headptr->p) {  /* merge */
            headptr->p = p, headptr->n += n;
            try_to_free(headptr);
            if(!headptr->n) {                       /* delete empty node */
                void *tp = headptr; headptr = headptr->next;
                free(tp);
            }
        }
        else {
            struct wtbheader *tp;
            if(! (tp = malloc(sizeof *tp)))/* can't save fragment, dump it */
                return;
            tp->p = p, tp->n = n;
            tp->next = headptr,    headptr = tp;
        }
    }    
    else {
        for(prevp = hp = headptr;
        hp->next && p > hp->next->p;
        prevp = hp, hp = hp->next)
            ;
        
        if((char*)hp->p + hp->n == p) {         /* merge to current */
            hp->n += n;
            try_to_free(hp);
            if(!hp->n) {                       /* delete empty node */
                if(hp == headptr)
                    headptr = NULL;
                prevp->next = hp->next;
                free(hp);
            }
        }
        else if(hp->next && (char *)p + n == hp->next->p) {/* merge to next */
            hp->next->p = p, hp->next->n += n;
            try_to_free(hp->next);
            if(!hp->next->n) {                      /* delete empty node */
                void *tp = hp->next;
                hp->next = hp->next->next;
                free(tp);
            }
        }
        else {                             /* insert */
            struct wtbheader *tp;
            if(! (tp = malloc(sizeof *tp)))/* can't save fragment, dump */
                return;
            tp->p = p, tp->n = n;
            tp->next = hp->next, hp->next = tp;
        }
    }
}

