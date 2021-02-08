/* Program 3.18: Adjacency-matrix graph representation
 * This program reads a set of edges that define an undirected graph and builds
 * an adjacency-matrix representation for the graph, setting a [i) [j] and
 * a[j] [i) to 1 if there is an edge from i to j or j to i in the graph, or to 0
 * if there is no such edge. The program assumes that the number of vertices V
 * is a compile-time constant. Otherwise, it would need to dynamically allocate
 * the array that represents the adjacency matrix (see Exercise 3.72).
 */
#include <stdio.h>
#include <stdlib.h>
#define V 8

int main() {
  int i, j, adj[V][V];

  for (i = 0; i < V; ++i) {
    for (j = 0; j < V; ++j)
      adj[i][j] = 0;
    adj[i][i] = 1;
  }
  while (scanf("%d %d\n", &i, &j) == 2) {
    adj[i][j] = 1;
    adj[j][i] = 1;
  }
  printf("  | 0 1 2 3 4 5 6 7\n");
  printf("-------------------\n");
  for (i = 0; i < V; ++i) {
    printf("%d | ", i);
    for (j = 0; j < V; ++j)
      printf("%d ", adj[i][j]);
    printf("\n");
  }
  return 0;
}
