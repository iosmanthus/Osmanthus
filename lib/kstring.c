#include <kstring.h>


// For the simplicity, we do not aplly loop unrolling

inline void *kmemcpy( void *dst, const void *src, u32 n )
{
  const u8 *s = (u8 *)src;
  u8 *t = (u8 *)dst;
  while ( n-- )
    *t++ = *s++;
  return dst;
}

inline void *kmemset( void *s, u8 c, u32 n )
{
  u8 *m = (u8 *)s;
  while ( n-- )
    *m++ = c;
  return s;
}

inline void *kbzero( void *s, u32 n )
{
  kmemset( s, '\0', n );
  return s;
}

inline i32 kstrcmp( const char *s1, const char *s2 )
{
  i32 a, b;
  do {
    a = *s1++;
    b = *s2++;
  } while ( a && b && a == b );
  return a - b;
}


inline i32 kstrncmp( const char *s1, const char *s2, u32 n )
{
  i32 a, b;
  for ( ; n && a == b; --n ) {
    a = *s1++;
    b = *s2++;
  }
  return a - b;
}

char *kstrcat( char *dst, const char *src )
{
  char *t = dst;
  while ( *t++ )
    ;
  --t;
  while ( *src )
    *t++ = *src++;
  return dst;
}

char *kstrncat( char *dst, const char *src, u32 n )
{
  char *t = dst;
  while ( *t++ )
    ;
  --t;
  while ( n-- )
    *t++ = *src++;
  *t = '\0';
  return dst;
}


char *kstrcpy( char *dst, const char *src )
{
  char *t = dst;
  while ( *src )
    *t++ = *src++;
  *t = '\0';
  return dst;
}
char *kstrncpy( char *dst, const char *src, u32 n )
{
  char *t = dst;
  u32 i;
  for ( i = 0; i < n && *src; ++i )
    *t++ = *src++;
  for ( ; i < n; ++i )
    *t++ = '\0';
  return dst;
}


u32 kstrlen( const char *s )
{
  u32 l = 0;
  while ( *s++ )
    ++l;
  return l;
}
