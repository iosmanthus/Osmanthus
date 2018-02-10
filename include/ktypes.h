#ifndef _KTYPES_H_
#define _KTYPES_H_


#ifndef NULL
#define NULL (void *)0
#endif

#ifndef EOF
#define EOF -1
#endif /* ifndef EOF */

/* Some type alias in 32-bit machine */
typedef unsigned int u32;
typedef int i32;
typedef unsigned short u16;
typedef short i16;
typedef unsigned char u8;
typedef char i8;

typedef enum _KDataSize {
  KBYTE = sizeof( u8 ),
  KWORD = sizeof( u16 ),
  KDWORD = sizeof( u32 )
} KDataSize;

#endif /* ifndef _KTYPES_H_ */
