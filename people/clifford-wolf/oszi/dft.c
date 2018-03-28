
#include <math.h>
#include <fftw3.h>

static double *d_in;
static fftw_complex *c_out;
static fftw_plan p;
int plan_n = -1;

void dft_wrapper(int n, const int *in, int *out)
{
	int i, j;

	if (n != plan_n) {
		if (plan_n >= 0) {
			fftw_destroy_plan(p);
			fftw_free((void*)d_in);
			fftw_free((void*)c_out);
		}
		d_in = (double*) fftw_malloc(sizeof(double) * n);
		c_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
		p = fftw_plan_dft_r2c_1d(n, d_in, c_out, FFTW_ESTIMATE);
		plan_n = n;
	}

	double maxval_in = 0;

	for (i=0; i<n; i++) {
		d_in[i] = in[i];
		if (fabs(d_in[i]) > maxval_in)
			maxval_in = fabs(d_in[i]);
	}

	fftw_execute(p);

	double maxval_out = 0;

	for (i=0; i<n/5; i++) {
		d_in[i] = sqrt(c_out[i][0]*c_out[i][0] + c_out[i][1]*c_out[i][1]);
		if (d_in[i] > maxval_out)
			maxval_out = d_in[i];
	}

	double in_to_out_factor = (2*maxval_in/maxval_out);

	for (i=0; i<n/5; i++) {
		double tmp = d_in[i] * in_to_out_factor;
		for (j=1; j<4; j++)
			out[i*5+j] = tmp;
		out[i*5] = out[i*5+4] = 0;
	}
}

