/* utvector
 * 
 * maintain a contiguous buffer of 'n' elements ('i' occupied)
 * the 'n' buffers are deep-inited at the time of allocation
 * the vector leaves popped slots as-is, clearing them on re-use
 * the memory management helper mm is used to define the size and
 * deep-init, deep-fini, deep-copy (into inited slots) and deep-clear.
 * deep-clear prepares a slot for re-use e.g. reset slot state.
 *
 */

#ifndef __UTVECTOR_H_
#define __UTVECTOR_H_

#include <stddef.h>

#define INITIAL_SIZE 16

typedef struct _UT_vector_mm {
   size_t sz;
   void (*init)(void *buf, unsigned num);             //-> utstring-init
   void (*fini)(void *buf, unsigned num);             //-> utstring-done
   void (*copy)(void *dst, void *src, unsigned num);  //-> ustring_concat
   void (*clear)(void *buf, unsigned num);            //-> utstring-clear
} UT_vector_mm;

typedef struct _UT_vector {
  UT_vector_mm mm;
  unsigned i,n;/* i: index of next available slot, n: num slots */
  char *d;     /* n slots of size icd->sz*/
} UT_vector;


UT_vector *utvector_new(UT_vector_mm *mm);
void utvector_init(UT_vector *v, UT_vector_mm *mm);
void utvector_reserve(UT_vector *v, unsigned num);
void utvector_fini(UT_vector *v);
UT_vector * utvector_clone(UT_vector *src);
void utvector_clear(UT_vector *v);
void utvector_copy(UT_vector *dst, UT_vector *src);
void utvector_free(UT_vector *v);
void *utvector_extend(UT_vector *v);
void *utvector_next(UT_vector *v, void *cur);
void utvector_pop(UT_vector *v);
void utvector_push(UT_vector *v, void *e);
unsigned utvector_len(UT_vector *v);

extern UT_vector_mm* utvector_int;

#endif /* __UTVECTOR_H_ */
