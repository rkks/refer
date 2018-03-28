#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
 
int n=16;
int verbose=0;
char *ifile=NULL;
char *tfile=NULL;
 
void usage(char *prog) {
  fprintf(stderr, "usage: %s [-n <2|4|8|...|32>] [-v] <source.txt> <test.txt>\n", prog);
  exit(-1);
}

unsigned hash_ber(char *in, size_t len) {
  unsigned hashv = 0;
  while (len--)  hashv = ((hashv) * 33) + *in++;
  return hashv;
}
unsigned hash_fnv(char *in, size_t len) {
  unsigned hashv = 2166136261UL;
  while(len--) hashv = (hashv * 16777619) ^ *in++;
  return hashv;
}
#define MASK(u,n) ( u & ((1UL << n) - 1))
#define NUM_HASHES 2
void get_hashv(char *in, size_t len, unsigned *out) {
  assert(NUM_HASHES==2);
  out[0] = MASK(hash_ber(in,len),n);
  out[1] = MASK(hash_fnv(in,len),n);
}

#define BIT_TEST(c,i) (c[i/8] & (1 << (i % 8)))
#define BIT_SET(c,i) (c[i/8] |= (1 << (i % 8)))
#define byte_len(n) (((1UL << n) / 8) + (((1UL << n) % 8) ? 1 : 0))
#define num_bits(n) (1UL << n)
char *bf_new(unsigned n) {
  char *bf = calloc(1,byte_len(n));
  return bf;
}
void bf_add(char *bf, char *line) {
  unsigned i, hashv[NUM_HASHES];
  get_hashv(line,strlen(line),hashv);
  for(i=0;i<NUM_HASHES;i++) BIT_SET(bf,hashv[i]);
}
void bf_info(char *bf, FILE *f) {
  unsigned i, on=0;
  for(i=0; i<num_bits(n); i++) 
    if (BIT_TEST(bf,i)) on++;

  fprintf(f, "%.2f%% saturation (%lu bits)\n", on*100.0/num_bits(n), num_bits(n));
}
int bf_hit(char *bf, char *line) {
  unsigned i, hashv[NUM_HASHES];
  get_hashv(line,strlen(line),hashv);
  for(i=0;i<NUM_HASHES;i++) {
    if (BIT_TEST(bf,hashv[i])==0) return 0;
  }
  return 1;
}
 
int main(int argc, char * argv[]) {
  int opt;
  FILE *ifilef=stdin,*tfilef=NULL;
  char line[100];
 
  while ( (opt = getopt(argc, argv, "n:v+")) != -1) {
    switch (opt) {
      case 'n':
        n = atoi(optarg);
        break;
      case 'v':
        verbose++;
        break;
      default:
        usage(argv[0]);
        break;
    }
  }
 
  if (optind < argc) ifile=argv[optind++];
  if (optind < argc) tfile=argv[optind++];
  if (!ifile || !tfile) usage(argv[0]);

  /* open files */
  if ( (ifilef = fopen(ifile,"r")) == NULL) {
    fprintf(stderr,"can't open %s: %s\n", ifile, strerror(errno));
    exit(-1);
  }
  if ( (tfilef = fopen(tfile,"r")) == NULL) {
    fprintf(stderr,"can't open %s: %s\n", tfile, strerror(errno));
    exit(-1);
  }

  /* make the bloom filter */
  char *bf= bf_new(n);
 
  /* loop over the source file */
  while (fgets(line,sizeof(line),ifilef) != NULL) bf_add(bf,line);

  /* print saturation etc */
  if (verbose) bf_info(bf,stderr); 

  /* now loop over the test file */
  int hit=0, miss=0;
  while (fgets(line,sizeof(line),tfilef) != NULL) {
    if (bf_hit(bf,line)) {
      hit++;
      if (verbose>1) fprintf(stderr,"hit on %s\n", line);
    } else {
      miss++;
    }
  }

  double hitrate = (hit+miss > 0) ? hit*100.0/(hit+miss) : 0;
  printf("%.2f%% hit rate (%u/%u) n=%u\n", hitrate, hit, hit+miss, n);
}
