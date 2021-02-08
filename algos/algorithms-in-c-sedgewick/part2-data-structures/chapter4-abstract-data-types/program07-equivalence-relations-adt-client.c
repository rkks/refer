/* Program 4.7: Equivalence-relations ADT client
 * The ADT of Program 4.6 separates the connectivity algorithm from the
 * union-find implementation, making that algorithm more accessible.
 */
#include <stdio.h>
#include <stdlib.h>
#include "UF.h"
int main(int argc, char *argv[]) {
  int p, q, N = atoi(argv[1]);
  UFinit(N);

  while (scanf("%d %d", &p, &q) == 2)
    if (!UFfind(p, q)) {
      UFunion(p, q);
      printf(" %d %d\n", p, q);
    }
  return 0;
}
