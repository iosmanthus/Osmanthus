#include <kports.h>


void kports_out( u16 ports, u32 data, u8 size )
{
  if ( size == BYTE )
    __asm__ volatile( "outb %1,%0" ::"d"( ports ), "a"( data ) );
  else if ( size == WORD )
    __asm__ volatile( "outw %1,%0" ::"d"( ports ), "a"( data ) );
  else if ( size == DWORD )
    __asm__ volatile( "outl %1,%0" ::"d"( ports ), "a"( data ) );
}

u8 kports_in( u16 ports )
{
  u8 val = 0;
  __asm__ volatile( "inb %1,%0" : "=a"( val ) : "d"( ports ) );
  return val;
}
