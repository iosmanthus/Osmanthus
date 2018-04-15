#ifndef _KATOMIC_H_
#define _KATOMIC_H_

#include <ktypes.h>

u32 kfectch_and_increase(u32 *addr);
void kincrease(u32 *addr);

#endif /* _KATOMIC_H_ */
