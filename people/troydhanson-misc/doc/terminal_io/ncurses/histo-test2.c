#include <stdio.h>
#include <pcap.h>
#include <assert.h>
#include "tpl.h"

char err[PCAP_ERRBUF_SIZE];
const int maxsz = 1024;
#define NBUCKETS 16
int buckets[NBUCKETS], npackets;

void report() {
  int i,d;
  tpl_node *tn = tpl_map("A(u)",&d);
  for(i=0; i < NBUCKETS; i++) {
    d = buckets[i];
    tpl_pack(tn,1);
    buckets[i]=0; /* reset */
  }
  tpl_dump(tn,TPL_FD,1); /* write to stdout */
  tpl_free(tn);
}

void cb(u_char *data, const struct pcap_pkthdr *hdr, const u_char *pkt) {
  //printf("packet of length %d\n", hdr->len);
  int bucket = (1.0 * hdr->caplen / maxsz) * NBUCKETS; assert(bucket < NBUCKETS);
  buckets[bucket]++;
  if (npackets++ >= 100) {report(); npackets=0;}
}

int main(int argc, char *argv[]) {
  pcap_t *p=NULL;
  int rc=-1;
  char *dev;

  dev = (argc > 1) ? argv[1] : pcap_lookupdev(err);
  if (dev == NULL) {
    fprintf(stderr, "no device: %s\n", err);
    goto done;
  }

  p = pcap_open_live(dev, maxsz, 1, 0, err);
  if (p == NULL) {
    fprintf(stderr, "can't open %s: %s\n", dev, err);
    goto done;
  }

  rc = pcap_loop(p, 0, cb, NULL);

 done:
  return rc;
}

