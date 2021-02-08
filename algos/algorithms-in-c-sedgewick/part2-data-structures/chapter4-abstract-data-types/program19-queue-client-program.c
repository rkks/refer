/* Program 4.19: Queue client program (queue simulation)
 * The availability of object handles makes it possible to build compound data
 * structures with ADT objects, such as the array of queues in this sample
 * client program, which simulates a situation where customers waiting for
 * service are assigned at random to one of M service queues.
 */
#include <stdio.h>
#include <stdlib.h>
#include "ItemInt.h"
#include "QUEUEfirstclass.h"
#define M 10

int main(int argc, char *argv[]) {
  int i, j, N = atoi(argv[1]);
  Q queues[M];

  for (i = 0; i < M; ++i)
    queues[i] = QUEUEinit(N);
  for (i = 0; i < N; ++i)
    QUEUEput(queues[rand()%M], i);
  for (i = 0; i < M; ++i, printf("\n"))
    for (j = 0; !QUEUEempty(queues[i]); ++j)
      printf("%3d ", QUEUEget(queues[i]));
  return 0;
}
