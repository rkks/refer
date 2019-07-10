#ifndef _STDDEV_H
#define _STDDEV_H
/*
 * Basic statistics module: standard deviation routines.

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
 */

struct stddev
{
	int min;
	int64_t sum;
	uint64_t sum_sq;
	uint64_t count;
};

static inline struct stddev *stddev_init(struct stddev *sd)
{
	sd->min = INT_MAX;
	sd->sum = 0;
	sd->sum_sq = 0;
	sd->count = 0;
	return sd;
}

static inline struct stddev *stddev_new()
{
	struct stddev *sd = malloc(sizeof(struct stddev));
	return stddev_init(sd);
}

static inline void stddev_free(struct stddev *sd) { free(sd); }

static inline void stddev_add(struct stddev *sd, int value)
{
	sd->count++;
	sd->sum += value;
	sd->sum_sq += value * value;
	if (value < sd->min) {
		sd->min = value;
	}
}

static inline void stddev_remove(struct stddev *sd, int old_value)
{
	sd->count--;
	sd->sum -= old_value;
	sd->sum_sq -= old_value * old_value;
}

static inline void stddev_modify(struct stddev *sd, int old_value,
				 int new_value)
{
	stddev_remove(sd, old_value);
	stddev_add(sd, new_value);
}

static inline void stddev_get(struct stddev *sd, uint64_t *counter_ptr,
			      double *avg_ptr, double *stddev_ptr)
{
	double avg = 0.0, dev = 0.0;
	double sum = (double)sd->sum, sum_sq = (double)sd->sum_sq,
	       count = (double)sd->count;

	if (avg_ptr) {
		if (sd->count != 0) {
			avg = sum / count;
		}
	}

	if (stddev_ptr) {
		if (sd->count > 2) {
			double variance =
				(sum_sq - (sum * (sum / count))) / count;
			dev = sqrt(fabs(variance));
		}
	}
	if (counter_ptr) {
		*counter_ptr = sd->count;
	}
	if (avg_ptr) {
		*avg_ptr = avg;
	}
	if (stddev_ptr) {
		*stddev_ptr = dev;
	}
}

static inline void stddev_merge(struct stddev *dst, struct stddev *a,
				struct stddev *b)
{
	dst->count = a->count + b->count;
	dst->sum = a->sum + b->sum;
	dst->sum_sq = a->sum_sq + b->sum_sq;
}

static inline void stddev_split(struct stddev *dst, struct stddev *a,
				struct stddev *b)
{
	dst->count = a->count - b->count;
	dst->sum = a->sum - b->sum;
	dst->sum_sq = a->sum_sq - b->sum_sq;
}

#endif // _STDDEV_H
