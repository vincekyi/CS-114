#include "Descriptor.h"
#ifndef _PONG_H_
#define _PONG_H_

typedef struct pong {
	struct descriptor desc;
	uint16_t port; 		/*	The TCP port number on which the
					responding host	can accept incoming
					Gnutella connections.
				*/
	uint32_t ipaddress; 	/*	The IPv4 address of the responding host
					BIG ENDIAN format
				*/
	uint32_t numFiles; 	/*	The number of files that the servent
					with the given IP Address and Port is
					sharing on the network.
				*/
	uint32_t numKilobytes;	/*	The number of kilobytes that the servent
					with the given IP Address and Port is
					sharing on the network.
				*/

	/* 	A Pong may have optional ping data, according to the 0.4
		protocol specification.  This is not supported here.
	*/
} pong_t;

#endif
