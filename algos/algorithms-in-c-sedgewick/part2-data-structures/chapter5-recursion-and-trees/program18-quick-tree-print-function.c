/* Program 5.18: Quick tree-print function
 * This recursive program keeps track of the tree height and uses that
 * information for indentation in printing out a representation of the
 * tree that we can use to debug tree-processing programs
 * (see Figure 5.29). It assumes that items in nodes are characters.
 *
 * It is an inorder traversal.
 */
void printnode(char c, int h) {
  int i;

  for (i = 0; i < h; ++i)
    printf(" ");
  printf("%c\n", c);
}

void show(link x, int h) {
  if (x == NULL) {
    printnode('*', h);
    return;
  }
  show(x->r, h+1);
  printnode(x->item, h);
  show(x->l, h+1);
}
