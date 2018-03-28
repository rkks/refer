#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <floatingpoint.h>
#include <siginfo.h>
#include <ucontext.h>
#include <ieeefp.h>
#include "ssp.h"

#ifndef BADSIG
#define BADSIG ((void (*)(void)) -1)
#endif

static void handler (int sig, siginfo_t *sip, ucontext_t *uap);

int main (void)
{
	float infinity;
	float num;
	fp_except saved_fsr;
	sigfpe_handler_type saved_handler;

	if ((saved_handler = sigfpe (FPE_FLTINV, handler)) == BADSIG)
		err_msg ("sigfpe failed");

	saved_fsr = fpgetmask ();
	fpsetmask (saved_fsr | FP_X_INV);

	infinity = 1.0 / (float) (getpid () - getpid ());
	num = infinity / infinity;

	fpsetmask (saved_fsr);
	sigfpe (FPE_FLTINV, saved_handler);

	return (0);
}

static void handler (int sig, siginfo_t *sip, ucontext_t *uap)
{
	char *exception;

	switch (sip -> si_code) {
		case FPE_FLTINV:
			exception = "Invalid operand";
			break;

		case FPE_FLTRES:
			exception = "Inexact";
			break;

		case FPE_FLTDIV:
			exception = "Division by 0";
			break;

		case FPE_FLTUND:
			exception = "Underflow";
			break;

		case FPE_FLTOVF:
			exception = "Overflow";
			break;

		default:
			exception = "Unknown";
			break;
	}

	printf ("FP exception caught: %s\n", exception);
	abort ();
}
