/*
 * The development of a string data type implementation similar to
 * Programs 6.9 and 6.10 is of particular interest, because character
 * strings are widely used as sort keys. Using the C library
 * string-comparison function, we can change the first three lines in
 * Program 6.9 to convert it to an interface for strings.
 */
typedef char *Item;
#define key(A) (A)
#define less(A, B) (strcmp(key(A), key(B)) < 0)
#define exch(A, B) { Item t = A; A = B; B = t; }
#define compexch(A, B) if (less(B, A)) exch(A, B)

int ITEMscan(Item *);
void ITEMshow(Item);
