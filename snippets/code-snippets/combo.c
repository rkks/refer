#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "combo.h"

// Example usage:  Generate words of length 5 with no letters repeated.
int main(void)
{
	int n, outlen=5;
	uint8_t *output;
	unsigned long int total=0;
	struct combo *c=comboloop_init("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26);
	if(c==NULL)
		return(1);

	// Get first result, configuring for length of 'curlen'.
	output=comboloop_pick(c, outlen);
	do
	{
		// Output is a general-purpose array, not a c-string,
		// so we can't assume it's null-terminated.
		// so we just write 5 bytes, then a newline.
		fwrite(output, 1, outlen, stdout);
		fputc('\n', stdout);
		// get next result, if any.
		output=comboloop_pick(c, -1);
	} while(output!=NULL); // If we run out of results, quit.

	fprintf(stderr, "%lu\n", c->n); // total count should be in c.

	comboloop_free(c);
	return(0);
}

void *memdup(const void *mem, size_t len);

void *comboloop_pick(struct combo *c, int len)
{
	int pos;
	uint8_t *out_set;
	uint8_t *set;

	if(len >= 0)
	{
		if(c->pick_out)	free(c->pick_out);
		c->pick_out=malloc(len * c->elem_size);
		if(c->cur_set == NULL)
			c->cur_set=malloc(c->elements * c->elem_size);

		memset(c->order, 0, sizeof(int) * c->elements);
		c->pick=len;
		c->n = 0;
		c->overflow=0;
	}
	else if((c->pick_out == NULL) || (c->pick <= 0) || (c->overflow))
		return(NULL);

	// Give us a new, full input set
	memcpy(c->cur_set, c->input_set, c->elements * c->elem_size);

	out_set=c->pick_out;
	set=((uint8_t *)c->cur_set);

	// Pick a combination of things
	for(pos=0; pos < c->pick; pos++)
	{
		uint8_t *last=set + ((c->elements-(pos+1)) * c->elem_size);
		uint8_t *pick=set + (c->order[pos] * c->elem_size);
		memcpy(out_set, pick, c->elem_size);
		out_set += c->elem_size;

		if(pick != last)
			memcpy(pick, last, c->elem_size);
	}

	for(pos=0; pos < c->pick; pos++)
	{
		c->order[pos]++;

		if(c->order[pos] >= (c->elements - pos))
		{
			c->order[pos]=0;
			if(pos == (c->pick - 1))
			{
				c->overflow=1;
				break;
			}

			continue;
		}

		break;
	}

	c->n++;
	return(c->pick_out);
}



void comboloop_free(struct combo *c)
{
	if(c != NULL)
	{
		if(c->input_set != NULL)	free(c->input_set);
		if(c->cur_set != NULL)		free(c->cur_set);
		if(c->pick_out != NULL)		free(c->pick_out);
		free(c);
	}
}

struct combo *comboloop_init(void *set, size_t elem_size, int len)
{
	struct combo *c=NULL;

	if((set == NULL) || (elem_size <= 0) || (len <= 0))
		goto COMBOLOOP_INIT_ERR;

	c=malloc(sizeof(struct combo)+(sizeof(int) * len));
	if(c == NULL)
		goto COMBOLOOP_INIT_ERR;

	memset(c, 0, sizeof(struct combo));

	c->input_set=memdup(set, len * elem_size);
	c->elem_size = elem_size;
	c->elements = len;
	return(c);

COMBOLOOP_INIT_ERR:
	comboloop_free(c);
	return(NULL);
}

void *memdup(const void *mem, size_t len)
{
	void *m;

	if(mem == NULL)	return(NULL);

	m=malloc(len);
	if(m == NULL)
		return(NULL);

	memcpy(m, mem, len);
	return(m);
}
