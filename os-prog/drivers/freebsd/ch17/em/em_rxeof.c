/* $Id: em_rxeof.c,v 1.5 2012/04/13 10:10:56 ghost Exp $ */

/*-
 * Copyright (c) 2001-2011 Intel Corporation.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

static bool
em_rxeof(struct rx_ring *rxr, int count, int *done)
{
	struct adapter *adapter = rxr->adapter;
	struct ifnet *ifp = adapter->ifp;
	struct e1000_rx_desc *cur;
	struct mbuf *mp, *sendmp;
	u8 status = 0;
	u16 len;
	int i, processed, rxdone = 0;
	bool eop;

	EM_RX_LOCK(rxr);

	for (i = rxr->next_to_check, processed = 0; count != 0; ) {
		if ((ifp->if_drv_flags & IFF_DRV_RUNNING) == 0)
			break;

		bus_dmamap_sync(rxr->rxdma.dma_tag, rxr->rxdma.dma_map,
		    BUS_DMASYNC_POSTREAD);

		mp = sendmp = NULL;
		cur = &rxr->rx_base[i];
		status = cur->status;
		if ((status & E1000_RXD_STAT_DD) == 0)
			break;
		len = le16toh(cur->length);
		eop = (status & E1000_RXD_STAT_EOP) != 0;

		if ((cur->errors & E1000_RXD_ERR_FRAME_ERR_MASK) ||
		    (rxr->discard == TRUE)) {
			++ifp->if_ierrors;
			++rxr->rx_discarded;
			if (!eop)
				rxr->discard = TRUE;
			else
				rxr->discard = FALSE;
			em_rx_discard(rxr, i);
			goto next_desc;
		}

		mp = rxr->rx_buffers[i].m_head;
		mp->m_len = len;
		rxr->rx_buffers[i].m_head = NULL;

		if (rxr->fmp == NULL) {
			mp->m_pkthdr.len = len;
			rxr->fmp = rxr->lmp = mp;
		} else {
			mp->m_flags &= ~M_PKTHDR;
			rxr->lmp->m_next = mp;
			rxr->lmp = mp;
			rxr->fmp->m_pkthdr.len += len;
		}

		if (eop) {
			--count;
			sendmp = rxr->fmp;
			sendmp->m_pkthdr.rcvif = ifp;
			++ifp->if_ipackets;
			em_receive_checksum(cur, sendmp);
#ifndef __NO_STRICT_ALIGNMENT
			if (adapter->max_frame_size >
			    (MCLBYTES - ETHER_ALIGN) &&
			    em_fixup_rx(rxr) != 0)
				goto skip;
#endif
			if (status & E1000_RXD_STAT_VP) {
				sendmp->m_pkthdr.ether_vtag =
				    le16toh(cur->special) &
				    E1000_RXD_SPC_VLAN_MASK;
				sendmp->m_flags |= M_VLANTAG;
			}
#ifndef __NO_STRICT_ALIGNMENT
skip:
#endif
			rxr->fmp = rxr->lmp = NULL;
		}

next_desc:
		cur->status = 0;
		++rxdone;
		++processed;

		if (++i == adapter->num_rx_desc)
			i = 0;

		if (sendmp != NULL) {
			rxr->next_to_check = i;
			EM_RX_UNLOCK(rxr);
			(*ifp->if_input)(ifp, sendmp);
			EM_RX_LOCK(rxr);
			i = rxr->next_to_check;
		}

		if (processed == 8) {
			em_refresh_mbufs(rxr, i);
			processed = 0;
		}
	}					/* The end of the for loop. */

	if (e1000_rx_unrefreshed(rxr))
		em_refresh_mbufs(rxr, i);

	rxr->next_to_check = i;
	if (done != NULL)
		*done = rxdone;
	EM_RX_UNLOCK(rxr);

	return ((status & E1000_RXD_STAT_DD) ? TRUE : FALSE);
}
