/* Program 5.21: Depth-first search
 * Recursive graph traversal, or depth-first search for systematically
 * visiting all nodes in a graph is a direct generalization of the
 * tree-traversal methods, and it serves as the basis for many basic
 * algorithms for processing graphs.
 *
 * It is a simple recursive algorithm: Starting at any node v, we
 *   1. visit v.
 *   2. (Recursively) visit each (unvisited) node attached to v.
 * If the graph is connected, we eventually reach all of the nodes.
 *
 * To visit all the nodes connected to node k in a graph, we mark it as
 * _visited_, then (recursively) visit all the unvisited nodes on k's
 * adjacency list.
 *
 * The difference between depth-first search and general tree traversal
 * (see Program 5.14) is that we need to guard explicitly against
 * visiting nodes that we have already visited.
 *
 * Property 5.10: Depth-first search requires time proportional to
 * V+E in a graph with V vertices and E edges, using the adjacency lists
 * representation.
 */
void traverse(int k, void (*visit)(int)) {
  link t;

  (*visit)(k);
  visited[k] = 1;
  for (t = adj[k]; t != NULL; t = t->next)
    if (!visited[t->v])
      traverse(t->v, visit);
}
