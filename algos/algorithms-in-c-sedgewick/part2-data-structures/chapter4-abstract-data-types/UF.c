/* Program 4.8: Equivalence-relations ADT implementation
 * This implementation of the weighted-quick-union code from Chapter 1,
 * together with the interface of Program 4.6, packages the code in a form that
 * makes in convenient for use in other applications. The implementation uses a
 * local function `find`.
 */
#include <stdlib.h>
#include "UF.h"
static int *id, *sz;

void UFinit(int N) {
  int i;
  id = malloc(N*sizeof(int));
  sz = malloc(N*sizeof(int));
  for (i = 0; i < N; ++i) {
    id[i] = i;
    sz[i] = 1;
  }
}

static int find(int x) {
  int i = x;
  
  while (i != id[i])
    i = id[i];
  return i;
}

int UFfind(int p, int q) {
  return (find(p) == find(q));
}

void UFunion(int p, int q) {
  int i = find(p), j = find(q);
  if (i == j)
    return;
  if (sz[i] < sz[j]) {
    id[i] = j;
    sz[j] += sz[i];
  } else {
    id[j] = i;
    sz[i] += sz[j];
  }
}
