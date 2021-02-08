/* Program 5.22: Breadth-first search
 * To visit all the nodes connected to node k in a graph, we put k onto
 * a FIFO queue, then enter into a loop where we get the next node from
 * the queue, and, if it has not been visited, visit it and push all the
 * unvisited nodes on its adjacency list, continuing until the queue is
 * empty.
 */
void traverse(int k, void (*visit)(int)) {
  link t;
  QUEUEinit(V);
  QUEUEput(k);

  while (!QUEUEempty())
    if (visited[k = QUEUEget()] == 0) {
      (*visit)(k);
      visited[k] = 1;
      for (t = adj[k]; t != NULL; t = t->next)
        if (visited[t->v] == 0)
          QUEUEput(t->v);
    }
}
