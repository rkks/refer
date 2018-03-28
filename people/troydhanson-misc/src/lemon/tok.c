#include <string.h>
#include "readcfg.h"
#include "cfg.h"

static struct {
  char *str;
  size_t len;
  int id;
} kw[] = 
{
 {"server",6, TOK_SERVER},
 {"name",4, TOK_NAME},
 {"port",4, TOK_PORT},
 {"{",1, TOK_LCURLY},
 {"}",1, TOK_RCURLY},
};
static const int ws[256] = { ['\r']=1, ['\n']=1, ['\t']=1, [' ']=1 };
static const int dg[256] = { ['0']=1, ['1']=1, ['2']=1, ['3']=1, ['4']=1, 
                             ['5']=1, ['6']=1, ['7']=1, ['8']=1, ['9']=1, };

int tok(char **c, size_t *bsz, size_t *toksz, int *line) {

  /* skip leading whitespace */
  while(*bsz && ws[(int)**c]) {if (**c=='\n') { (*line)++; } (*bsz)--; (*c)++;}
  if (*bsz == 0) return 0; // end of input

  /* identity literal keywords */
  int i;
  for(i=0; i < sizeof(kw)/sizeof(*kw); i++) {
    if (*bsz < kw[i].len) continue;
    if (memcmp(*c,kw[i].str,kw[i].len) ) continue;
    *toksz = kw[i].len;
    return kw[i].id;
  }

  /* digits ending with end-of-buffer or whitespace */
  *toksz=0; 
  while ((*toksz < *bsz) && dg[(int)*(*c+*toksz)]) (*toksz)++;
  if (*toksz && ((*toksz==*bsz) || (ws[(int)*(*c+*toksz)]))) return TOK_NUMBER; 

  /* alphanumeric with dots ending with end-of-buffer or whitespace */
  *toksz=0; 
  while ((*toksz < *bsz) && (dg[(int)*(*c+*toksz)] || 
         ((*(*c+*toksz) >= 'a') && (*(*c+*toksz) <= 'z')) || 
         ((*(*c+*toksz) >= 'A') && (*(*c+*toksz) <= 'Z')) ||
         (*(*c+*toksz) == '.') )) (*toksz)++;
  if (*toksz && ((*toksz==*bsz) || (ws[(int)*(*c+*toksz)]))) return TOK_DOTALN; 

  return -1;
}
