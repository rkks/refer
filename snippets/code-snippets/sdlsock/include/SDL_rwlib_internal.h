/**
 * Macros helpful for creating an SDL_RWops wrapper for something
 */
#ifndef __SDL_RWLIB_INTERNAL_H__
#define __SDL_RWLIB_INTERNAL_H__

#define RW_TYPE(RW)  ((RW)->type)
#define RW_DATAINT(RW) ((RW)->hidden.stdio.autoclose)
#define RW_DATAPTR1(RW) ((RW)->hidden.mem.here)
#define RW_DATAPTR2(RW) ((RW)->hidden.mem.here)

#define RWNAME(PKG,Y) rw_ ## PKG ## _ ## Y
#define RWFNAME(PKG,Y) RWNAME(PKG,Y)
#define PREAMBLE(PKG,Y) static int RWNAME(PKG,Y)(SDL_RWops *ctx

#define RWFUNC_SEEK(PKG)  PREAMBLE(PKG,seek) ,int offset, int whence)
#define RWFUNC_READ(PKG)  PREAMBLE(PKG,read) ,void *ptr, int size, int  maxnum)
#define RWFUNC_WRITE(PKG) PREAMBLE(PKG,write),const void *ptr, int size, int maxnum)
#define RWFUNC_CLOSE(PKG) PREAMBLE(PKG,close))

#define RW_TYPENUM2(PKG) RWLIB_NUM_ ## PKG
#define RW_TYPENUM(PKG) RW_TYPENUM2(PKG)

#define RW_INITFUNC(RW,PKG) { \
  (RW)->seek=RWNAME(PKG,seek); \
  (RW)->read=RWNAME(PKG,read); \
  (RW)->write=RWNAME(PKG,write); \
  (RW)->close=RWNAME(PKG,close); \
  (RW)->type=RW_TYPENUM(PKG); }

#define RWFUNC_WRITE_STUB(PKG) RWFUNC_WRITE(PKG) { \
  SDL_SetError("%s:  Writing unimplimented",__FUNCTION__); return(-1); }

#define RWFUNC_SEEK_STUB(PKG) RWFUNC_SEEK(PKG) { \
  SDL_SetError("%s:  Seeking unimplimented",__FUNCTION__); return(-1); }

typedef enum rwlib_num_e
{
  RWLIB_NUM_DONOTHING=0xdead0000,
  RWLIB_NUM_FD,
  RWLIB_NUM_SOCK,
  RWLIB_NUM_GZ,
  RWLIB_NUM_TMP,
  RWLIB_NUM_BZ2,
  RWLIB_NUM_RING,
  RWLIB_NUM_HTTP,
  RWLIB_NUM_LAST
} rwlib_num_e;

#endif/*__SDL_RWLIB_INTERNAL_H__*/
