#include <stdio.h>
#include "utf.h"

/* 
 * test converting utf8 <--> utf32 
 * we use `iconv` library call to confirm our results
 *
 */
int main() {
  /* convert a string of utf8 to utf32 and back*/
  char *orig = "¢ hello cent ¢ euro € beijing 北京";
  uint32_t utf32[100], *u32=utf32;
  char *c;
  printf("original:    %s\n", orig);
  for(c=orig; *c; c+=utf8_charlen(*c)) {
    *u32++ = utf8_to_32(c);
  }
  *u32 = 0; /* 'terminate' the utf32 integers */

  /*write the raw utf32 to stderr. compare it to 
      echo <orig> | iconv -f UTF8 -t UTF32 
    but ignore the initial BOM inserted by iconv */
  /* for(u32=utf32;*u32;u32++) write(2,u32,sizeof(uint32_t)); */

  /* convert each utf32 back to utf8, print*/
  printf("utf8->32->8: ");
  for(u32=utf32; *u32; u32++) {
    uint8_t utf8[4];
    size_t sz = utf32_to_8(*u32, utf8);
    printf("%.*s", (unsigned)sz, utf8);
  }
  printf("\n");
}
