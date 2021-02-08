This table gives implementations of basic list-processing operations with five
commonly used conventions. This type of code is used in simple applications
where the list-processing code is inline.

### Circular, never empty
* first insert: `head->next = head;`
* insert t after x: `t->next = x->next; x->next = t;`
* delete after x: `x->next = x->next->next;`
* traversal loop: `t = head; do { ... ; t = t->next;} while (t != head);`
* test if one item: `if (head->next == head)`

### Head pointer, null tail
* initialize: `head = NULL;`
* insert t after x:  
```c
if (x == NULL) { head = t; head->next = NULL; }
else { t->next = x->next; x->next = t; }
```
* delete after x: `t = x->next; x->next = t->next;`
* traversal loop: `for (t = head; t != NULL; t = t->next)`
* test if empty: `if (head == NULL)`

### Dummy head node, null tail
* initialize: `head = malloc(sizeof *head); head->next = NULL;`
* insert t after x: `t->next = x->next; x->next = t;`
* delete after x: `t = x->next; x->next = t->next;`
* traversal loop: `for (t = head->next; t != NULL; t = t->next)`
* test if empty: `if (head->next == NULL)`

### Dummy head and tail nodes
* initialize: `head = malloc(sizeof *head);`  
  `z = malloc(sizeof *z);`  
  `head->next = z; z->next = z;`
* insert t after x: `t->next = x->next; x->next = t;`
* delete after x: `x->next = x->next->next;`
* traversal loop: `for (t = head->next; t != z; t = t->next)`
* test if empty: `if (head->next != z)`
