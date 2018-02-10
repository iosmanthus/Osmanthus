#include <kport.h>


void kout( u16 port, u32 data, KDataSize size )
{
  if ( size == KBYTE )
    __asm__ volatile( "outb %%al,%%dx" ::"d"( port ), "a"( data ) );
  else if ( size == KWORD )
    __asm__ volatile( "outw %%ax,%%dx" ::"d"( port ), "a"( data ) );
  else if ( size == KDWORD )
    __asm__ volatile( "outl %%eax,%%dx" ::"d"( port ), "a"( data ) );
}

u32 kin( u16 port, KDataSize size )
{
  u8 db = 0;
  u16 dw = 0;
  u32 dl = 0;
  if ( size == KBYTE ) {
    __asm__ volatile( "inb %%dx, %%al" : "=a"( db ) : "d"( port ) );
    return db;
  } else if ( size == KWORD ) {
    __asm__ volatile( "inw %%dx, %%ax" : "=a"( dw ) : "d"( port ) );
    return dw;
  } else if ( size == KDWORD ) {
    __asm__ volatile( "inl %%dx, %%eax" : "=a"( dl ) : "d"( port ) );
    return dl;
  }
  return 0;
}
