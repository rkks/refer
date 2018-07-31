/* bcast.c :
 *
 * Broadcast Ticker Updates
 */
#include "quotes.h"

void
broadcast(
  int s,                  /* Socket */
  TickReq *quote,          /* Quote */
  struct sockaddr *bc_addr, /* addr */
  socklen_t bc_len) {  /* addr len. */
    int z;                /* Status */
    char buf[2048];       /* Buffer */
    char *cp = buf;     /* Buf. ptr */
    int msglen;   /* Message length */

    /*
     * Format a datagram for broadcast:
     */
    strcpy(buf,quote->ticker);
    cp = buf + strlen(buf) + 1;
    sprintf(cp,"%E",quote->last_trade);
    cp += strlen(cp) + 1;
    strcpy(cp,quote->date);
    cp += strlen(cp) + 1;
    strcpy(cp,quote->time);
    cp += strlen(cp) + 1;
    sprintf(cp,"%E",quote->change);
    cp += strlen(cp) + 1;
    sprintf(cp,"%E",quote->open_price);
    cp += strlen(cp) + 1;
    sprintf(cp,"%E",quote->high);
    cp += strlen(cp) + 1;
    sprintf(cp,"%E",quote->low);
    cp += strlen(cp) + 1;
    sprintf(cp,"%E",quote->volume);
    cp += strlen(cp) + 1;
    
    msglen = cp - buf;

    /*
     * Broadcast the datagram :
     */
    z = sendto(s,buf,msglen,0,bc_addr,bc_len);
    if ( z == -1 )
        msgf('e',"%s: sendto(2)",
            strerror(errno));
}
