#include <stdio.h>
#include <assert.h>
#include "utarray.h"
#include "tpl.h"

#define EDGES 6  // hexagons

unsigned get_hash(unsigned char *s) {
  unsigned h=0;
  while(*s != '\0') h=h*33 + *s++; // Bernstein
  return h;
}

#define odd(i) (i&1)
void get_hextile_neighbor(int x, int y, int edge, int *ox, int *oy) {
  switch(edge) {
    /*  e */ case 0: *ox =          x+1;       *oy = y+0; break;
    /* ne */ case 1: *ox = odd(y) ? x+1 : x+0; *oy = y-1; break;
    /* nw */ case 2: *ox = odd(y) ? x+0 : x-1; *oy = y-1; break;
    /*  w */ case 3: *ox =          x-1;       *oy = y+0; break;
    /* sw */ case 4: *ox = odd(y) ? x+0 : x-1; *oy = y+1; break;
    /* se */ case 5: *ox = odd(y) ? x+1 : x+0; *oy = y+1; break;
    default: assert(0); break;
  }
}

/* given an initial hexagon at (xv[pos],yv[pos]),
   is the neighbor at the given edge occupied? */
int available(int /* item index */ pos,
              int /* neighbor at */edge,
              UT_array /* of int */*xv,
              UT_array /* of int */*yv,
              int /* output coord*/*ox,
              int /* output coord*/*oy) {
  int x = *(int*)utarray_eltptr(xv,pos); // own position: x
  int y = *(int*)utarray_eltptr(yv,pos); // own position: y
  get_hextile_neighbor(x,y,edge,ox,oy);  // neighbor[edge]=[ox,oy]

  // scan through xv/yv to see if ox/oy used. TODO hash lookup.
  int p;
  for(p=0; p < utarray_len(xv); p++) {
    x = *(int*)utarray_eltptr(xv,p);
    y = *(int*)utarray_eltptr(yv,p);
    if ((x == *ox) && (y == *oy)) return 0;
  }
  fprintf(stderr,"attaching to pos:%d/%d\n",pos,edge);
  return 1;
}


void place( int /* item index */ pos,
            UT_array /*of int */*hashv,
            UT_array /*of int */*xv,
            UT_array /*of int */*yv) {
  int x=0, y=0, e, p;
  if (pos == 0) goto done; // place first one at origin

  /*prefer to attach this hexagon to predecessor edge based on
    this one's hashcode%6, but if that's occupied, increment
    through successive edges looking for an open neighbor. If
    none are found, look backward from predecessor to origin
    hexagon, scanning their edges in the same way, until an 
    open neighbor is found. it is certain to find an open one. */
  unsigned my_hash = *(unsigned*)utarray_eltptr(hashv,pos);
  for (p = pos-1; p >= 0; p--) { // hunt through predecessor chain
    for(e=0; e < 6; e++) {       // hunt through predecessor edges
      unsigned bond_edge = (my_hash + e) % EDGES;
      fprintf(stderr," +trying %d/%d\n",p,bond_edge);
      if (available(p,bond_edge,xv,yv,&x,&y)) goto done;
      fprintf(stderr," -taken, edge scanning\n");
    }
  }
  assert(0); // not reached. it's a math thing.

 done:
  utarray_push_back(xv, &x);
  utarray_push_back(yv, &y);
}

int layout(char *buf, size_t sz, char **obuf, size_t *osz) {
  UT_array *namev,*hashv, *xv, *yv;
  int i, pos, rc=-1, x, y;
  unsigned char *s;
  unsigned h;

  utarray_new(namev, &ut_str_icd);
  utarray_new(hashv, &ut_int_icd);
  utarray_new(xv, &ut_int_icd);
  utarray_new(yv, &ut_int_icd);

  tpl_node *tn = tpl_map("A(s)", &s);
  if (tpl_load(tn, TPL_MEM, buf, sz)) goto done;
  while (tpl_unpack(tn,1) > 0) {
    h = get_hash(s);
    utarray_push_back(namev, &s);
    utarray_push_back(hashv, &h);
    free(s);
  }
  tpl_free(tn);

  for(pos=0; pos < utarray_len(namev); pos++) {
    place(pos,hashv,xv,yv);
  }

  /* every item has a place. prepare output. */
  tn = tpl_map("A(sii)", &s, &x, &y);
  for(i=0; i < utarray_len(namev); i++) {
    s = *(unsigned char**)utarray_eltptr(namev,i);
    x =            *(int*)utarray_eltptr(xv,i);
    y =            *(int*)utarray_eltptr(yv,i);
    tpl_pack(tn,1);
  }
  tpl_dump(tn, TPL_MEM, obuf, osz);
  tpl_free(tn);

  rc = 0;

 done:
  utarray_free(namev);
  utarray_free(hashv);
  utarray_free(xv);
  utarray_free(yv);
  return rc;
}
