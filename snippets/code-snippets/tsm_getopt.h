#ifndef __TSM_GETOPT_H__
#define __TSM_GETOPT_H__

/**
 * tsm_getopt (C) 2008 Tyler Montbriand, tsm@accesscomm.ca.  Freeware.
 * I wrote this because, for all of its complexity, I couldn't find a
 * way to make GNU getopt to actuallhy *DO* anything with the data it 
 * spent so much trouble processing.  
 */

typedef struct opt
{
	const char	*lname;		// Long option, mandatory.
	unsigned char	sname;		// Short option, optional.
	int		args;		// Has arguments.
						// When cmdstr is NULL, stores
						// pointer to arg in *val
						// when cmdstr isn't NULL,
						// scanf's string into val
	void 		*val;		// What to store
	const char	*cmdstr;	// Command string for scanf
	const char	*usage;		// Usage description
} opt;

#ifdef __cplusplus
extern "C" {
#endif

// Parse commandline parameters.  Returns index of first unparsed value.
int tsm_getopt(int argc, char *argv[], const opt options[]);
// Print usage for optional parameters
int tsm_usage(int argc, char *argv[], const opt options[]);

#ifdef __cplusplus
}
#endif

#endif/*__TSM_GETOPT_H__*/
