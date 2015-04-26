#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <poll.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>

#include "rlib.h"

struct reliable_state {
    rel_t *next;			/* Linked list for traversing all connections */
    rel_t **prev;

    conn_t *c;			/* This is the connection object */

    /* Add your own data fields below this */

	/*client fields
		enum client_state  (wait_for_ack, wait_for_stdin, all packets send,..)  
		last_ack_rec - seqno of last ackno received; 
			note: this is really the seqno of the last ackno packet that was received! 
			not as in ackno of struct packets where the index is shifted by 1
			the same applies to last_pack_sent,last_pack_ack,...
		last_pack_sent - seqno of last packet sent 
	
	
	*/ 
	
	
	/*server fields
		enum server_state  (wait for packet, all packets acknowledged,...) 
		last_pack_ack - seqno of last packet acknowledged
		last_pack_rec - seqno of last packet received 
	
	
	
	*/ 
};
rel_t *rel_list;





/* Creates a new reliable protocol session, returns NULL on failure.
* ss is always NULL */
rel_t *
rel_create (conn_t *c, const struct sockaddr_storage *ss,
const struct config_common *cc)
{
    rel_t *r;

    r = xmalloc (sizeof (*r));
    memset (r, 0, sizeof (*r));

    if (!c) {
        c = conn_create (r, ss);
        if (!c) {
            free (r);
            return NULL;
        }
    }

    r->c = c;
    r->next = rel_list;
    r->prev = &rel_list;
    if (rel_list)
    rel_list->prev = &r->next;
    rel_list = r;

    /* Do any other initialization you need here */
	
	
    return r;
}

void
rel_destroy (rel_t *r)
{
    if (r->next)
        r->next->prev = r->prev;
    *r->prev = r->next;
    conn_destroy (r->c);

    /* Free any other allocated memory here */
}


void
rel_recvpkt (rel_t *r, packet_t *pkt, size_t n)
{
	//validate packet//
	if (valid_packet(pkt,n))
	{
		//ack or data packet// 
		if(ntohs (pkt->len) == 8)
		{
			/*
				either: 
				waiting for an ack packet
						if received ackno is higher than last ackno received, then state updated  
						if(pkt->ackno - 1 > r->last_ack_rec)
						{
							r->last_ack_rec = pkt->ackno; 
							r->client_state = waiting_for_stdin;
							rel_read 
						}
				waiting for an eof ack packet 
						TO DO 
			*/ 
					
		}
		else
		{/*
						either: 
					packet has been received and processed before 
							last_pack_ack >= seqno then send ack  
					packet is next in order packet and server is waiting for data packets 
							last_pack_ack + 1 == seqno then read packet, write to stdout 
									is an eof 
											packet -> len == 12 
											send eof to output (conn_out len=0)
							                create and send ack_packet with seqno 
											close connection if client has finished sending 
									if not eof : 
							 				write data to output (conn_out)
											create and send ack_packet
											update rel state (last_pack_ack, last_ack_rec)*/ 
												
		}
	
	}
	
}


void
rel_read (rel_t *s)
{
}

void
rel_output (rel_t *r)
{
}

void
rel_timer ()
{
    /* Retransmit any packets that need to be retransmitted */

}



/*------------------------------------------------------------------------
below: functions that are needed for several rel_....*/ 


//checks if a packet has been transmitted correctly, returns 1 if so, 0 else // 
int
valid_packet (packet_t *pkt, size_t n)
{
	//examine length: received size n should equal the length of the packet // 
	
	if(n == (size_t) ntohs (pkt->len))
	{
		//compute checksum (set cksum to 0 and use cksum function to compute the value that should be in cksum)
		uint16_t checksum = pkt-> cksum; 
		memset(pkt->cksum, 0, sizeof(pkt->cksum)); 
		if(cksum(pkt,n) == checksum)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{return 0;}
	
}








