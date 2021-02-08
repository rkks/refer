#include "linkedlist.h"

int count_links(link root) {
  int cnt = 0;
  link nd;

  for (nd = root; nd->next != root; nd = nd->next, ++cnt)
    ;
  return cnt;
}

int count_links_between(link start, link end) {
  int cnt = 0;
  link nd;

  for (nd = start; nd->next != end; nd = nd->next, ++cnt)
    ;
  return cnt;
}

link connect_lists(link x, link t) {
  link temp;

  temp = x->next;
  x->next = t;
  while (t->next != x->next)
    t = t->next;
  t->next = temp;
  return x;
}

void move_node(link t, link x) {
  link t_prev = t;

  while (t_prev->next != t)
    t_prev = t_prev->next;
  t_prev->next = t->next;
  t->next = x->next;
  x->next = t;
}
