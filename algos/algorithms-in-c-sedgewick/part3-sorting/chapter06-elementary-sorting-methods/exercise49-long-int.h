/* Exercise 6.49: Write an interface and implementation for the generic
 * item datatype (similar to Programs 6.9 and 6.10) to support having
 * the sorting methods sort long integers
 */
typedef long Item;
#define key(A) (A)
#define less(A, B) (key(A) < key(B))
#define exch(A, B) { Item t = A; A = B; B = t; }
#define compexch(A, B) if (less(B, A)) exch(A, B)

Item ITEMrand(void);
int ITEMscan(Item *);
void ITEMshow(Item);
