/* Program 6.7: Interface for array data type
 * This Array.h interface defines high-level functions for arrays of
 * abstract items: Initialize random values, initialize values read from
 * standard input, print the contents, and sort the contents. The item
 * types are defined in a separate interface (see Program 6.9).
 */
void randinit(Item[], int);
void scaninit(Item[], int *);
void show(Item[], int, int);
void sort(Item[], int, int);
