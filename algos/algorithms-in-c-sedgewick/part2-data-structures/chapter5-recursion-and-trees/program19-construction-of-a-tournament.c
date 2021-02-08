/* Program 5.19: Construction of a tournament
 * This recursive function divides a file a[l], ..., a[r] into the two
 * parts a[l], ..., a[m] and a[m+1], ..., a[r], builds tournaments for
 * the two parts (recursively), and makes a tournament for the whole
 * file by setting links in a new node to the recursively built
 * tournaments and setting its item value to the larger of the items in
 * the roots of the two recursively built tournament.
 *
 * It is a modification of Program 5.6.
 *
 * The very operation that we use to build the tournament is an
 * important example: Given two tournaments, we can combine them into a
 * single tournament in constant time, by creating a new node making
 * its left link point to one of the tournaments and its right link
 * point to the other, and taking the larger of the two items
 * (at the roots of the two given tournaments) as the largest item in
 * the combined tournament.
 */
typedef struct node *link;
struct node {
  Item item;
  link l, r;
};

link NEW(Item item, link l, link r) {
  link x = malloc(sizeof *x);
  x->item = item;
  x->l = l;
  x->r = r;
  return x;
}

link max(Item a[], int l, int r) {
  int m = (l+r)/2;
  Item u, v;

  link x = NEW(a[m], NULL, NULL);
  if (l == r)
    return x;
  x->l = max(a, l, m);
  x->r = max(a, m+1, r);

  u = x->l->item;
  v = x->r->item;
  if (u > v)
    x->item = u;
  else
    x->item = v;
  return x;
}
