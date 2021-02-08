/* Program 6.9: Sample interface for item data type
 * The file Item.h that is included in Programs 6.6 and 6.8 defines the
 * data representation and assoicated operations for the items to be
 * sorted. In this example, the items are floating-point keys. We use
 * macros for the key, less, exch, and compexch data type operations for
 * use by our sorting programs; we could also define them as functions
 * to be implemented separtely, like the three functions ITEMrand
 * (return a random key), ITEMscan(read a key from standard input) and
 * ITEMshow (print the value of a key).
 */
typedef double Item;
#define key(A) (A)
#define less(A, B) (key(A) < key(B))
#define exch(A, B) { Item t = A; A = B; B = t; }
#define compexch(A, B) if (less(B, A)) exch(A, B)

Item ITEMrand(void);
int ITEMscan(Item *);
void ITEMshow(Item);
