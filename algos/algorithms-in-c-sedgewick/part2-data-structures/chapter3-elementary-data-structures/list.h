/* Program 3.12: List-processing interface
 * This code, which might be kept in an interface file `list.h`, specifies the
 * types of nodes and links, and declares some of the operations that we might
 * want to perform on them. We declare our own functions for allocating and
 * freeing memory for list nodes. The function `initNodes` is for the
 * convenience of the implementation. The `typedef` for `Node` and the
 * functions `Next` and `Item` allow clients to use lists without dependence
 * upon implementation details
 */
typedef struct node* link;
struct node {
  int item;
  link next;
};
typedef link Node;

void initNodes(int);
link newNode(int);
void freeNode(link);
void insertNext(link, link);
link deleteNext(link);
link Next(link);
int Item(link);
