#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "tconf.h"

static const unsigned char ws[256] = {[' ']=1,['\t']=1};
static const unsigned char nl[256] = {['\r']=1,['\n']=1};

int tconf(char *file, tconf_t *tconf, int tclen, int opt) {
  
  char line[200],*k,*v, *kt, *vt, *tmp;
  int rc = -1,klen,vlen,re;
  tconf_func_t fptr;
  FILE *f=NULL;
  tconf_t *t;

  if ( (f = fopen(file,"r")) == NULL) {
    fprintf(stderr,"can't open %s: %s\n", file, strerror(errno));
    rc = -1;
    goto done;
  }

  while (fgets(line,sizeof(line),f) != NULL) {
    k=line; while(ws[*k]) k++;                           /* trim pre space */
    if ((*k == '#') || nl[*k] || (*k=='\0')) continue;   /* skip comments */
    v=k; while(!(ws[*v] || nl[*v] || (*v=='\0'))) v++;   /* find k/v delim */
    klen = v-k;
    while(ws[*v]) v++;                                   /* trim pre space */
    vlen = 0; while (!(nl[*(v+vlen)] || (*(v+vlen)=='\0'))) vlen++;
    while(vlen && ws[*(v+vlen-1)]) vlen--;               /* trim post space */

    for(t = tconf; t < tconf+tclen; t++) {
      if ((klen != strlen(t->name)) || (strncmp(k,t->name,klen))) continue;
      switch(t->type) {
        case tconf_bool:
          if (vlen) {
            if (sscanf(v,"%d",(int*)(t->addr)) != 1) goto done;
            *(int*)(t->addr) = (*(int*)(t->addr))  ? 1 : 0;
          } else {
            *(int*)(t->addr) = 1;  /* lone key name means boolean true */
          }
          break;
        case tconf_int:
          if (!vlen) goto done;
          if (sscanf(v,"%d",(int*)(t->addr)) != 1) goto done;
          break;
        case tconf_str:
          if (!vlen) goto done;
          if ( (*(char**)(t->addr) = malloc(vlen+1)) == NULL) goto done;
          strncpy(*(char**)(t->addr),v,vlen);
          (*(char**)(t->addr))[vlen]='\0';
          break;
        case tconf_func:
          fptr = (tconf_func_t)t->addr;
          if ( (tmp = malloc(vlen+1+klen+1)) == NULL) goto done;
          kt = &tmp[0]; if (klen) memcpy(kt, k, klen); kt[klen] = '\0';
          vt = &tmp[klen+1]; if (vlen) memcpy(vt, v, vlen); vt[vlen] = '\0';
          re = fptr(kt,vt);
          free(tmp);
          if (re) goto done;
          break;
        default:
          fprintf(stderr,"unknown tconf type %d\n",tconf->type);
          goto done;
      }
    }
  }


  rc = 0; /* success */

 done:
  if (f) fclose(f);
  return rc;
}
