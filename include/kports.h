#ifndef _KPORTS_H_
#define _KPORTS_H_

#include <ktypes.h>

#ifndef BYTE
#define BYTE ( (u8)1 )
#endif /* ifndef BYTE */

#ifndef WORD
#define WORD ( ( u8 )( BYTE << 1 ) )
#endif /* ifndef WORD */

#ifndef DWORD
#define DWORD ( ( u8 )( WORD << 1 ) )
#endif /* ifndef DWORD */

void kports_out( u16 port, u32 data, u8 size ); // Write data
u8 kports_in( u16 port );                       // Read data

#endif /* ifndef _KPORTS_H_ */
