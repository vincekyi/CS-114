#include "Descriptor.h"
#ifndef _PING_H_
#define _PING_H_

typedef struct ping {
	struct descriptor desc;

	/* 	A Ping may have optional ping data, according to the 0.4
		protocol specification.  This is not supported here.
	*/	
} ping_t;

#endif
