/* Program 6.6: Sort driver for arrays
 * This driver for basic array sorts uses two explicit interfaces:
 * One for the functions that initialize and print (and sort!) arrays,
 * and the other for a data type that encapsulates the operations that
 * we perform on generic items. The first allows us to compile the
 * functions for arrays separately and perhaps to use them in other
 * drivers; the second allows us to sort other types of data with the
 * same sort code.
 */
#include <stdlib.h>
#include "Item.h"
#include "Array.h"

int main(const int argc, char *argv[]) {
	int i, N = atoi(argv[1]), sw = atoi(argv[2]);
	Item *a = malloc(N*sizeof(Item));
	if (sw)
		randinit(a, N);
	else
		scaninit(a, &N);
	sort(a, 0, N-1);
	show(a, 0, N-1);
}
