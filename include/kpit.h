#ifndef _KPIT_H_
#define _KPIT_H_

#include <kisr.h>
#include <ktypes.h>

void init_timer( u32 frequency, KInterruptHandler handler );

#endif /* ifndef _KPIT_H_ */
