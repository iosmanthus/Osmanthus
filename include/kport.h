#ifndef _KPORTS_H_
#define _KPORTS_H_

#include <ktypes.h>

/*
 * Write 'data' to 'port' while specifying the data size: 'size'
 * */
void kout( u16 port, u32 data, KDataSize size );


/*
 * Read 'size' byte(s) from 'port'
 * */
u32 kin( u16 port, KDataSize size );


#endif /* ifndef _KPORTS_H_ */
