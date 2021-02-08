/* Program 5.5: Examples of recursive functions for linked lists
 * These recursive functions for simple list-processing tasks are easy to
 * express, but may not be useful for huge lists because the depth of the
 * recursion may be proportional to the length of the list.
 * The first function, count, counts the number of nodes on the list.
 * The second, traverse, calls the function visit for each node on the
 * list, from beginning to end. These two functions are both also easy to
 * implement with a for or while loop. The third function, traverseR,
 * does not have a simple iterative counterpart. It calls the function visit
 * for every node on the list, but in reverse order.
 * The fourth function, delete, makes the structural changes needed
 * for a given item to be deleted from a list. It returns a link to the
 * (possibly altered) remainder of the list-the link returned is x, except
 * when x->i tern is v, when the link returned is x->next (and the recursion
 * stops).
 */

int count(link x) {
  if (x == NULL)
    return 0;
  return 1 + count(x->next);
}

void traverse(link h, void (*visit)(link)) {
  if (h == NULL)
    return;
  (*visit)(h);
  traverse(h->next, visit);
}

void traverseR(link h, void (*visit)(link)) {
  if (h == NULL)
    return;
  traverseR(h->next, visit);
  (*visit)(h);
}

link delete(link x, Item v) {
  if (x == NULL)
    return NULL;
  if (eq(x->item, v)) {
    link t = x->next;
    free(x);
    return t;
  }
  x->next = delete(x->next, v);
  return x;
}
