/* Program 6.12: Data-type interface for record items
 * The record have two keys: A string key (e.g., a name) in the first
 * field, and an integer (e.g., a grade) in the second field. The
 * comparison less is defined as a function, rather than as a macro, so
 * we can change sort keys by changing implementations.
 */
#define maxN 30
struct record {
	char name[30];
	int num;
};
typedef struct record* Item;
#define exch(A, B) { Item t = A; A = B; B = t; }
#define compexch(A, B) if (less(B, A)) exch(A, B);
int less(Item, Item);
Item ITEMrand();
int ITEMscan(Item *);
void ITEMshow(Item);
