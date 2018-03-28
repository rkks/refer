#ifndef __COMBO_H__
#define __COMBO_H__

/**
 *	Routines for generating permutations of strings.
 *	Given "abcdefghijklmnopqrstuvwxyz" it will generate
 *	possible words without repeated letters.
 */

struct combo
{
	void *input_set;
	void *cur_set;
	void *pick_out;

	int elem_size, elements, pick, overflow;
	unsigned long int n;
	int order[0];
};

/**
 * Initializes a comboloop structure in prep for being looped over.
 *	set: 	a block of binary data specifying the input set.
 *		i.e. "abcd", an element size of 1, and a len of 4 would work,
 *		or an array of integers with an element size of sizeof(int),
 *		etc.
 *	elem_size:
 *		the size of each element in the array.  for an array
 *		of characters, the size would be 1.
 *	len:	the number of elements in the array.
 */
struct combo *comboloop_init(void *set, size_t elem_size, int len);
/**
 * If you want words of 5 length, call comboloop_pick(c, 5), then
 * thereafter call comboloop_pick(c, -1) until it starts returning
 * NULL.
 * You don't need to free the results it gives you, but they will
 * not be valid after c is freed or comboloop_pick is called again.
 */
void *comboloop_pick(struct combo *c, int len);
/**
 * Free the structure and all it contains, including any results it
 * returned.
 */
void comboloop_free(struct combo *c);

#endif/*__COMBOLOOP_H__*/
