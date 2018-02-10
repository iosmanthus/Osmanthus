#ifndef _KSTRING_H_
#define _KSTRING_H_

#include <ktypes.h>

void *kmemcpy( void *dst, const void *src, u32 n );

void *kmemset( void *s, u8 c, u32 n );
void *kbzero( void *s, u32 n );

i32 kstrcmp( const char *s1, const char *s2 );
i32 kstrncmp( const char *s1, const char *s2, u32 n );

char *kstrcpy( char *dst, const char *src );
char *kstrncpy( char *dst, const char *src, u32 n );

char *kstrcat( char *dest, const char *src );
char *kstrncat( char *dest, const char *src, u32 n );

u32 kstrlen( const char *s );

#endif /* ifndef _KSTRING_H_ */
