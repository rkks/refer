#include "libut.h"

static void utstring_init_wrapper(void *_buf, unsigned num) {
  UT_string *buf = (UT_string*)_buf;
  unsigned i;
  for(i=0; i < num; i++) {
    utstring_init(buf);
    buf++;
  }
}

static void utstring_done_wrapper(void *_buf, unsigned num) {
  UT_string *buf = (UT_string*)_buf;
  unsigned i;
  for(i=0; i < num; i++) {
    utstring_done(buf);
    buf++;
  }
}

static void utstring_clear_wrapper(void *_buf, unsigned num) {
  UT_string *buf = (UT_string*)_buf;
  unsigned i;
  for(i=0; i < num; i++) {
    utstring_clear(buf);
    buf++;
  }
}

static void utstring_copy_wrapper(void *_dst, void *_src, unsigned num) {
  UT_string *dst = (UT_string*)_dst;
  UT_string *src = (UT_string*)_src;
  unsigned i;
  for(i=0; i < num; i++) {
    utstring_concat(dst,src);
    dst++;
    src++;
  }
}

/* a few basic vector types as described via mm that can be passed to utvector_init/new */
static UT_vector_mm utvector_utstring_mm = {
  .sz = sizeof(UT_string),
  .init = utstring_init_wrapper,
  .fini = utstring_done_wrapper,
  .copy = utstring_copy_wrapper,
  .clear = utstring_clear_wrapper
};
UT_vector_mm* utvector_utstring = &utvector_utstring_mm;

