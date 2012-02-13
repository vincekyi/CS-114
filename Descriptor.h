#ifndef _DESCRIPTOR_H_
#define _DESCRIPTOR_H_

#define PING 0x00
#define PONG 0x01
#define QUERY 0x80
#define QUERYHITS 0x81
#define PUSH 0x40
#define Bye 0x02
#define IBCM 0x10
#define QRP 0x30
#define OVE 0x31
#define SVE 0x32

#define MAX_TTL 127

typedef struct descriptor {
	char descriptorID[16]; /*	128-bit (16-byte) Descriptor ID.
					Servents can use the UUID generation
					algorithm or use a cryptographically
					strong random generator.  The value of
					the Descriptor ID carries no
					signification, and should always be
					treated as an opaque binary string,
					whose byte order must be preserved when
					forwarding messages.
				*/
	uint8_t payloadDescriptor; /*	Ping, Pong, Query, QueryHits, Push, Bye,
					IBCM, QRP, OVE, SVE
				*/
	uint8_t TTL;		/*	Time To Live.  When TTL reaches 0, the
					descriptor MUST no longer be forwarded.
					A value higher than 127 is excessive.
				*/
	uint8_t hops;		/*	The number of times the descriptor has
					been forwarded.
					TTL(i) + Hops(i) = TTL(0)
					TTL(i+1) < TTL(i)
					Hops(i+1) > Hops(i)
				*/
	uint32_t payloadLength;	/*	The length of the descriptor immediately
					following this header.  The next
					descriptor header is located exactly
					Payload Length bytes from the end of
					this header (no gaps). 2GB is excessive.
				*/
} descriptor_t;

#endif
