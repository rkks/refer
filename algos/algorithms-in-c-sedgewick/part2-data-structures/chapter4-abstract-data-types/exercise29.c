/* Exercise 4.29: Modify Program 4.8 to use an array of structures instead of
 * parallel arrays for the underlying data structure.
 */
#include <stdlib.h>
#include "UF.h"
typedef struct _data {
  int id;
  int sz;
} Data;
static Data *data;

void UFinit(int N) {
  int i;
  data = malloc(N*sizeof(Data));
  for (i = 0; i < N; ++i) {
    data[i].id = i;
    data[i].sz = 1;
  }
}

static int find(int x) {
  int i = x;
  
  while (i != data[i].id)
    i = data[i].id;
  return i;
}

int UFfind(int p, int q) {
  return (find(p) == find(q));
}

void UFunion(int p, int q) {
  int i = find(p), j = find(q);
  if (i == j)
    return;
  if (data[i].sz < data[j].sz) {
    data[i].id = j;
    data[j].sz += data[i].sz;
  } else {
    data[j].id = i;
    data[i].sz += data[j].sz;
  }
}
