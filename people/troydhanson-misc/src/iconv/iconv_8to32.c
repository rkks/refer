#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This is an example of using the iconv library.
 * There is a command line interface to libiconv, simply called
 * "iconv". You can run it with the -l option to list the supported
 * charsets. Or use it to convert like "iconv -f UTF8 -t UTF32 < txt"
 */

int main() {
  char *orig = "¢ hello cent ¢ euro € beijing 北京", *utf8=orig;
  size_t orig_len = strlen(orig);

  char utf32[100], *u32 = utf32;;
  size_t utf32_len = sizeof(utf32);

  iconv_t ic = iconv_open("UTF-32LE", "UTF8"); /* TO, FROM */
  if (ic == (iconv_t)-1) {
    fprintf(stderr,"iconv_open failed: %s\n", strerror(errno));
    exit(-1);
  }

  if (iconv(ic, &utf8, &orig_len, &u32, &utf32_len) == (size_t)-1) {
    fprintf(stderr,"iconv failed: %s\n", strerror(errno));
    exit(-1);
  }

  /* now there are (sizeof(utf32)-utf32_len) bytes of utf32 */
  write(2, utf32, (sizeof(utf32)-utf32_len));

  iconv_close(ic);
}
