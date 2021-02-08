/* Program 5.15: Preorder traversal (nonrecursive)
 * This nonrecursive stack-based function is functionally equivalent to
 * its recursive counterpart, Program 5.14.
 */
void traverse(link h, void (*visit)(link)) {
  STACKinit(max);
  STACKpush(h);
  while(!STACKempty()) {
    (*visit)(h = STACKpop());
    if (h->r != NULL)
      STACKpush(h->r);
    if (h->l != NULL)
      STACKpush(h->l);
  }
}
