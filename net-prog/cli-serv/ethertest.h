/*
 *
 * Definitions that are used in server and client
 *
 */

#include <linux/if_ether.h> 

/* see linux/if_ether.h */
#define ETH_MAC_LEN        ETH_ALEN      /* Octets in one ethernet addr   */
#define ETH_HEADER_LEN     ETH_HLEN      /* Total octets in header.       */
#define ETH_MIN_FRAME_LEN  ETH_ZLEN      /* Min. octets in frame sans FCS */
#define ETH_USER_DATA_LEN  ETH_DATA_LEN  /* Max. octets in payload        */
#define ETH_MAX_FRAME_LEN  ETH_FRAME_LEN /* Max. octets in frame sans FCS */

#define ETH_FRAME_TOTALLEN 1518          /*Header: 14 + User Data: 1500 FCS: 4*/

#define DEVICE             "eth0"	/* Device used for communication*/

#define ETH_P_NULL	   0x0		/* we are running without any protocol above the Ethernet Layer*/
