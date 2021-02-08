/* Program 5.16: Level-order traversal
 * Switching the underlying data structure in preorder traversal (see
 * Program 5.15) from a stack to a queue transforms the traversal into
 * a level-order one.
 */
void traverse(link h, void (*visit)(link)) {
  QUEUEinit(max);
  QUEUEput(h);
  while (!QUEUEempty()) {
    (*visit)(h = QUEUEget());
    if (h->l != NULL)
      QUEUEput(h->l);
    if (h->r != NULL)
      QUEUEput(h->r);
  }
}
