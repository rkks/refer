/* Exercise 5.14: Recursive tree traversal
 * This recursive function takes a link to a tree as an argument and
 * calls the function `visit` with each of the nodes in the tree as
 * argument. As is, the function implements a preorder traversal; if we
 * move the call to `visit` between the recursive calls, we have an
 * inorder traversal; and if we move the call to `visit` after the
 * recursive calls, we have a postorder traversal.
 */
void traverse(link h, void (*visit)(link)) {
  if (h == NULL)
    return;
  (*visit)(h);
  traverse(h->l, visit);
  traverse(h->r, visit);
}
